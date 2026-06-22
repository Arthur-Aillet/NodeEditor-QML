#pragma once

#include "CutieWindow.hpp"
#include <glib.h>
#include <gst/gl/gl.h>
#include <gst/gstelement.h>
#include <gst/gstutils.h>

#include <qquickitem.h>
#include <qrunnable.h>

class GstreamerController {
  public:
  GstreamerController(CutieWindow *window);
  ~GstreamerController();

  void linkQtSink(QQuickItem *item);
  void unlinkQtSink(QQuickItem *item);

  private:
  void linkQtSinkAfterQML(QQuickItem *item, GstElement *sink);

  public:
  GstElement *pipeline;
  GstElement *qmlsink;
  GstElement *tee;
  GstElement *src;

  GstElement *fakesink;
  GstElement *queueFake;
  GstPad *blockpad;
  QQuickItem *item;

  private:
  CutieWindow *_window;
};