#pragma once

#include <qevent.h>
#include <qlogging.h>
#include <qquickwindow.h>

class CutieWindow : public QQuickWindow {
  Q_OBJECT
  QML_ELEMENT
  public:
  void mouseMoveEvent(QMouseEvent *event) override {
    QQuickWindow::mouseMoveEvent(event);
    emit mouseMoveEventForward(event);
  }

  void resizeEvent(QResizeEvent *event) override {
    QQuickWindow::resizeEvent(event);
    emit resizeEventForward(event);
  }

  void startRequestRefresh(QObject *object) {
    if (_dependsOnRefresh.empty()) {
      QObject::connect(this, &QQuickWindow::frameSwapped, this, &QQuickWindow::requestUpdate);
    }
    _dependsOnRefresh.push_back(object);
  }

  void stopRequestRefresh(QObject *object) {
    auto pos = std::find(_dependsOnRefresh.begin(), _dependsOnRefresh.end(), object);
    if (pos != _dependsOnRefresh.end())
      _dependsOnRefresh.erase(pos);
    if (_dependsOnRefresh.empty()) {
      QObject::disconnect(this, &QQuickWindow::frameSwapped, this, &QQuickWindow::requestUpdate);
    }
  }

  signals:
  void mouseMoveEventForward(QMouseEvent *event);
  void resizeEventForward(QResizeEvent *event);

  protected:
  std::vector<QObject *> _dependsOnRefresh = {};
};
