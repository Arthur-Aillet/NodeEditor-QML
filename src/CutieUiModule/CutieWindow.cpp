#include "CutieWindow.hpp"
#include <qqmlcontext.h>

CutieWindow *CutieWindow::getCutieWindow(QQmlEngine *engine) {
  auto app = qvariant_cast<QApplication *>(engine->rootContext()->contextProperty("app"));

  return getCutieWindow(app);
};

CutieWindow *CutieWindow::getCutieWindow(QApplication *application) {
  for (auto w : application->allWindows()) {
    CutieWindow *cutieWindow = qobject_cast<CutieWindow *>(w);
    if (cutieWindow != nullptr)
      return cutieWindow;
  }
  return nullptr;
};

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
