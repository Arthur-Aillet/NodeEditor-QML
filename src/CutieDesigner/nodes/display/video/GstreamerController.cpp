#include "GstreamerController.hpp"
#include "glib.h"
#include "gst/gl/gl-enumtypes.h"
#include "gst/gstbin.h"
#include "gst/gstcaps.h"
#include "gst/gstelement.h"
#include "gst/gstutils.h"
#include <cstddef>
#include <qlogging.h>
#include <qquickitem.h>
#include <qvariant.h>

static GstBusSyncReply on_sync_bus_message(GstBus *bus, GstMessage *msg, gpointer data) {
  GstreamerController *ctrl = (GstreamerController *)(data);

  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_HAVE_CONTEXT: {
    GstContext *context;

    gst_message_parse_have_context(msg, &context);

    /* if you need specific behviour or a context from a specific element,
     * you need to be selective about which context's you set on the
     * pipeline */
    if (gst_context_has_context_type(context, GST_GL_DISPLAY_CONTEXT_TYPE)) {
      gst_println("got have-context %p", context);
      gst_element_set_context(ctrl->pipeline, context);
    }

    if (context)
      gst_context_unref(context);
    gst_message_unref(msg);
    return GST_BUS_DROP;
  }
  default:
    break;
  }

  return GST_BUS_PASS;
}

GstreamerController::GstreamerController(CutieWindow *window) : _window(window) {
  pipeline = gst_pipeline_new(NULL);

  src = gst_element_factory_make("v4l2src", NULL);
  g_object_set(src, "device", "/dev/video0", NULL);
  GstElement *jpegdec = gst_element_factory_make("jpegdec", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);
  tee = gst_element_factory_make("tee", "t");
  queueFake = gst_element_factory_make("queue", "fakeQueue");
  fakesink = gst_element_factory_make("fakesink", NULL);

  if (!pipeline || !src || !tee || !queueFake || !fakesink) {
    g_error("Failed to create elements");
  }

  gst_bin_add_many(GST_BIN(pipeline), src, jpegdec, videoconvert, tee, glupload, queueFake,
                   fakesink, NULL);
  // GstCaps *caps =
  //     gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 1280, "height", G_TYPE_INT, 720,
  //                         "framerate", GST_TYPE_FRACTION, 30, 1, "texturetarget",
  //                         GST_TYPE_GL_TEXTURE_TARGET, GST_GL_TEXTURE_TARGET_2D,
  //                         //"stream-format", G_TYPE_STRING, "byte-stream",
  //                         // NULL);
  //                         "format", G_TYPE_STRING, "RGBA", NULL);

  if (!gst_element_link_many(src, jpegdec, videoconvert, tee, NULL)) {
    g_error("Failed to link elements");
  }
  if (!gst_element_link_many(tee, queueFake, glupload, fakesink, NULL)) {
    g_error("Failed to link elements 2");
  }

  GstBus *bus = gst_element_get_bus(pipeline);
  gst_bus_set_sync_handler(bus, on_sync_bus_message, this, NULL);
  gst_object_unref(bus);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // QString pipelineSrc = "v4l2src ";
  // pipelineSrc.append(" ! glupload ");
  // // pipelineSrc.append("! glcolorconvert ");
  // pipelineSrc.append("! glcolorconvert name=glcolorconvert ");
  // pipelineSrc.append("! queue name=queue ");

  // qDebug() << "Pipeline base: " << pipelineSrc;
  // GError *err = NULL;
  // pipeline = gst_parse_launch(pipelineSrc.toStdString().c_str(), &err);
  // if (!pipeline) {
  //   qCritical() << "Pipeline creation failed " << err;
  // }

  // GstBus *bus = gst_element_get_bus(pipeline);
  // gst_bus_set_sync_handler(bus, on_sync_bus_message, this, NULL);
  // gst_object_unref(bus);

  // fakesink = gst_element_factory_make("fakesink", "fakesink");
  // g_object_set(fakesink, "sync", TRUE, NULL);

  // qmlsink = gst_element_factory_make("qml6glsink", "qml6sink");
  // videoconvert = gst_bin_get_by_name(GST_BIN(pipeline), "glcolorconvert");

  // // videoconvert = gst_element_factory_make("glcolorconvert", "glcolorconvert");

  // auto queue = gst_bin_get_by_name(GST_BIN(pipeline), "queue");
  // blockpad = gst_element_get_static_pad(queue, "src");

  // gst_bin_add(GST_BIN(pipeline), fakesink);
  // gst_element_link(queue, fakesink);
  // gst_object_unref(queue);

  // if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
  //   qCritical() << "Pipeline failed to start";
  // }
}

GstreamerController::~GstreamerController() {
  if (pipeline) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(blockpad);
    gst_object_unref(pipeline);
  }
}

void GstreamerController::linkQtSinkAfterQML(QQuickItem *item, GstElement *sink) {
  g_object_set(sink, "widget", gpointer(item->property("videoItem").value<QQuickItem *>()), NULL);
  gst_element_set_state(sink, GST_STATE_PLAYING);
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // qDebug() << "Linking QQuickItem";
  //  gst_element_set_state(fakesink, GST_STATE_NULL);

  // GST_DEBUG_OBJECT(pipeline, "removing %" GST_PTR_FORMAT, fakesink);
  // gst_bin_remove(GST_BIN(pipeline), fakesink);

  // GST_DEBUG_OBJECT(pipeline, "adding   %" GST_PTR_FORMAT, qmlsink);
  // gst_bin_add(GST_BIN(pipeline), qmlsink);

  // auto videoconvert = gst_bin_get_by_name(GST_BIN(pipeline), "videoconvert");
  // gst_element_link(videoconvert, qmlsink);
  // g_object_set(qmlsink, "widget", item, NULL);
  // gst_object_unref(videoconvert);

  // if (gst_element_set_state(qmlsink, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
  //   qCritical() << "qmlsink failed to start";
  // }
}

static GstPadProbeReturn event_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
  // GstreamerController *ctrl = (GstreamerController *)user_data;

  // gst_element_set_state(ctrl->fakesink, GST_STATE_NULL);

  // GST_INFO_OBJECT(ctrl->pipeline, "removing %" GST_PTR_FORMAT, ctrl->fakesink);
  // gst_bin_remove(GST_BIN(ctrl->pipeline), ctrl->fakesink);

  // GST_INFO_OBJECT(ctrl->pipeline, "adding   %" GST_PTR_FORMAT, ctrl->qmlsink);
  // gst_bin_add(GST_BIN(ctrl->pipeline), ctrl->qmlsink);

  // auto queue = gst_bin_get_by_name(GST_BIN(ctrl->pipeline), "queue");
  // // gst_element_link(queue, ctrl->qmlsink);
  // gst_element_link(queue, ctrl->qmlsink);
  // g_object_set(ctrl->qmlsink, "widget", ctrl->item, NULL);
  // gst_object_unref(queue);

  // qCritical() << "Change Over";

  // if (gst_element_set_state(ctrl->videoconvert, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
  //   qCritical() << "videoconvert failed to start";
  // }

  // if (gst_element_set_state(ctrl->qmlsink, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
  //   qCritical() << "qmlsink failed to start";
  // }

  // if (gst_element_set_state(ctrl->pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
  //   qCritical() << "Pipeline failed to start";
  // }

  // return GST_PAD_PROBE_DROP;
}

static GstPadProbeReturn pad_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
  // gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

  // GstreamerController *ctrl = (GstreamerController *)user_data;

  // auto queue = gst_bin_get_by_name(GST_BIN(ctrl->pipeline), "queue");

  // GstPad *srcpad = gst_element_get_static_pad(queue, "src");
  // gst_pad_add_probe(
  //     srcpad, (GstPadProbeType)(GST_PAD_PROBE_TYPE_BLOCK | GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM),
  //     event_probe_cb, user_data, NULL);
  // gst_object_unref(queue);

  // /* push EOS into the element, the probe will be fired when the
  //  * EOS leaves the effect and it has thus drained all of its data */
  // GstPad *sinkpad = gst_element_get_static_pad(queue, "sink");
  // gst_pad_send_event(sinkpad, gst_event_new_eos());
  // gst_object_unref(sinkpad);

  // gst_object_unref(queue);
  // return GST_PAD_PROBE_OK;
}

void GstreamerController::linkQtSink(QQuickItem *_item) {
  item = _item;

  GstPad *sinkpad, *teepad;
  GstPadTemplate *templ;

  qDebug() << "Linking Qt";

  gst_element_set_state(pipeline, GST_STATE_PAUSED);
  // GstElement *glupload = gst_element_factory_make("glupload", NULL);

  templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(tee), "src_%u");

  // GstCaps *caps = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 640, "height",
  // G_TYPE_INT,
  //                                     480, "framerate", GST_TYPE_FRACTION, 30, 1,
  //                                     "texture-target", GST_TYPE_GL_TEXTURE_TARGET,
  //                                     GST_GL_TEXTURE_TARGET_2D,
  //                                     //"stream-format", G_TYPE_STRING, "byte-stream",
  //                                     NULL);
  // MJPG
  // GstCaps *caps = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 1280, "height",
  //                                     G_TYPE_INT, 720, "framerate", GST_TYPE_FRACTION, 30,
  //                                     1, //"stream-format", G_TYPE_STRING, "byte-stream",
  //                                     "format", G_TYPE_STRING, "RGB", NULL);

  // YUYV
  // GstCaps *caps = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 640, "height",
  // G_TYPE_INT,
  //                                     480, "framerate", GST_TYPE_FRACTION, 30,
  //                                     1, //"stream-format", G_TYPE_STRING, "byte-stream",
  //                                     "format", G_TYPE_STRING, "YUY2", NULL);

  //  teepad = gst_element_request_pad(tee, templ, NULL, caps);
  teepad = gst_element_request_pad(tee, templ, NULL, NULL);
  GstElement *queueQml = gst_element_factory_make("queue", "qmlQueue");
  GstElement *videoscale = gst_element_factory_make("videoscale", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);
  GstElement *glcolorscale = gst_element_factory_make("glcolorscale", NULL);
  GstElement *glcolorconvert = gst_element_factory_make("glcolorconvert", NULL);

  // GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *sink = gst_element_factory_make("qml6glsink", NULL);
  // g_object_set(sink, " force-aspect-ratio", false, NULL);

  gst_bin_add_many(GST_BIN(pipeline), queueQml, glupload, glcolorscale, glcolorconvert, sink, NULL);
  // gst_element_link_many(glupload, sink, NULL);
  gst_element_link_many(queueQml, glupload, glcolorscale, glcolorconvert, sink, NULL);

  // gst_element_link_filtered(queueQml, glupload,
  //                           gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 1280,
  //                           "height",
  //                                               G_TYPE_INT, 720, "framerate", GST_TYPE_FRACTION,
  //                                               30, 1, //"stream-format", G_TYPE_STRING,
  //                                               "byte-stream", "format", G_TYPE_STRING, "YUY2",
  //                                               NULL));

  // gst_element_link_filtered(queueQml, glupload, gst_caps_new_simple("video/x-raw", NULL));

  sinkpad = gst_element_get_static_pad(queueQml, "sink");
  gst_pad_link(teepad, sinkpad);
  gst_object_unref(sinkpad);

  // sinkpad = gst_element_get_static_pad(conv, "sink");

  // gst_pad_add_probe(blockpad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, pad_probe_cb, this, NULL);
  _window->scheduleRenderJob(
      QRunnable::create([this, _item, sink]() { linkQtSinkAfterQML(_item, sink); }),
      QQuickWindow::AfterSynchronizingStage);
}

void GstreamerController::unlinkQtSink(QQuickItem *item) { qDebug() << "..."; }