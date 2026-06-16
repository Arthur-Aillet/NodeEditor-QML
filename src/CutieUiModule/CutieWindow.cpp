#include "CutieWindow.hpp"

void CutieWindow::mouseMoveEvent(QMouseEvent *event) {
  QQuickWindow::mouseMoveEvent(event);
  emit mouseMoveEventForward(event);
}

void CutieWindow::resizeEvent(QResizeEvent *event) {
  QQuickWindow::resizeEvent(event);
  emit resizeEventForward(event);
}

void CutieWindow::startRequestRefresh(QObject *object) {
  if (_dependsOnRefresh.empty()) {
    QObject::connect(this, &QQuickWindow::frameSwapped, this, &QQuickWindow::requestUpdate);
  }
  _dependsOnRefresh.push_back(object);
}

void CutieWindow::stopRequestRefresh(QObject *object) {
  auto pos = std::find(_dependsOnRefresh.begin(), _dependsOnRefresh.end(), object);
  if (pos != _dependsOnRefresh.end())
    _dependsOnRefresh.erase(pos);
  if (_dependsOnRefresh.empty()) {
    QObject::disconnect(this, &QQuickWindow::frameSwapped, this, &QQuickWindow::requestUpdate);
  }
}
