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
  void linkQtSinkAfterQML(QQuickItem *item);

  public:
  GstElement *pipeline = nullptr;
  GstElement *qmlsink = nullptr;
  GstElement *tee = nullptr;
  GstElement *src = nullptr;

  GstElement *fakesink = nullptr;
  GstElement *queueFake = nullptr;

  private:
  CutieWindow *_window = nullptr;
};