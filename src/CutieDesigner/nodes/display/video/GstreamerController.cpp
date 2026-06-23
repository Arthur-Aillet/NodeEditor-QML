#include "GstreamerController.hpp"
#include "glib.h"
#include "gst/gstbin.h"
#include "gst/gstelement.h"
#include "gst/gstutils.h"
#include <cstddef>
#include <qlogging.h>
#include <qquickitem.h>
#include <qvariant.h>

static GstBusSyncReply onSyncBusMessage(GstBus *bus, GstMessage *msg, gpointer data) {
  GstElement *pipeline = (GstElement *)(data);

  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_HAVE_CONTEXT: {
    GstContext *context;

    gst_message_parse_have_context(msg, &context);

    /* if you need specific behviour or a context from a specific element,
     * you need to be selective about which context's you set on the
     * pipeline */
    if (gst_context_has_context_type(context, GST_GL_DISPLAY_CONTEXT_TYPE)) {
      gst_element_set_context(pipeline, context);
    }

    if (context) {
      gst_context_unref(context);
    }
    gst_message_unref(msg);
    return GST_BUS_DROP;
  }
  default:
    break;
  }
  return GST_BUS_PASS;
}

static void controlGLContext(GstElement *pipeline) {
  GstBus *bus = gst_element_get_bus(pipeline);
  gst_bus_set_sync_handler(bus, onSyncBusMessage, pipeline, NULL);
  gst_object_unref(bus);
}

GstreamerController::GstreamerController(CutieWindow *window) : _window(window) {
  pipeline = gst_pipeline_new(NULL);

  src = gst_element_factory_make("v4l2src", NULL);
  g_object_set(src, "device", "/dev/video0", NULL);
  GstElement *jpegdec = gst_element_factory_make("jpegdec", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  tee = gst_element_factory_make("tee", "t");
  queueFake = gst_element_factory_make("queue", "fakeQueue");
  fakesink = gst_element_factory_make("fakesink", NULL);

  if (!pipeline || !src || !tee || !queueFake || !fakesink) {
    g_error("Failed to create elements");
  }

  gst_bin_add_many(GST_BIN(pipeline), src, jpegdec, videoconvert, tee, queueFake, fakesink, NULL);

  if (!gst_element_link_many(src, jpegdec, videoconvert, tee, queueFake, fakesink, NULL)) {
    g_error("Failed to link elements");
  }
  controlGLContext(pipeline);
}

GstreamerController::~GstreamerController() {
  qDebug() << "Destructor" << qmlsink << pipeline;

  if (pipeline) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }
}

void GstreamerController::linkQtSinkAfterQML(QQuickItem *item) {
  g_object_set(qmlsink, "widget", gpointer(item->property("videoItem").value<QQuickItem *>()),
               NULL);
  gst_element_set_state(qmlsink, GST_STATE_PLAYING);
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void GstreamerController::linkQtSink(QQuickItem *_item) {
  gst_element_set_state(pipeline, GST_STATE_PAUSED);

  GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(tee), "src_%u");

  GstElement *queueQml = gst_element_factory_make("queue", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);
  qmlsink = gst_element_factory_make("qml6glsink", NULL);

  GstPad *teepad = gst_element_request_pad(tee, templ, NULL, NULL);

  gst_bin_add_many(GST_BIN(pipeline), queueQml, glupload, qmlsink, NULL);
  gst_element_link_many(queueQml, glupload, qmlsink, NULL);

  GstPad *sinkpad = gst_element_get_static_pad(queueQml, "sink");
  gst_pad_link(teepad, sinkpad);
  gst_object_unref(sinkpad);

  _window->scheduleRenderJob(QRunnable::create([this, _item]() { linkQtSinkAfterQML(_item); }),
                             QQuickWindow::AfterSynchronizingStage);
}

static GstPadProbeReturn unlink_cb(GstPad *pad, GstPadProbeInfo *info, gpointer data) {
  GstreamerController *ctrl = (GstreamerController *)data;
  qDebug() << "Unlink CB";

  gst_element_set_state(ctrl->pipeline, GST_STATE_PAUSED);
  g_object_set(ctrl->qmlsink, "widget", NULL, NULL);
  gst_element_set_state(ctrl->qmlsink, GST_STATE_NULL);

  return GST_PAD_PROBE_REMOVE;
}

void GstreamerController::unlinkQtSink(QQuickItem *item) {
  qDebug() << "Unlink" << qmlsink << pipeline << item;

  GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(tee), "src_%u");
  GstPad *teepad = gst_element_request_pad(tee, templ, NULL, NULL);
  gst_pad_add_probe(teepad, GST_PAD_PROBE_TYPE_IDLE, unlink_cb, this, NULL);
}