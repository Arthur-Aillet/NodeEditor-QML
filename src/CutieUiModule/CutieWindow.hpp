#pragma once

#include <QtWidgets/qapplication.h>
#include <qevent.h>
#include <qqmlengine.h>
#include <qquickwindow.h>

class CutieWindow : public QQuickWindow {
  Q_OBJECT
  QML_ELEMENT
  public:
  void mouseMoveEvent(QMouseEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

  void startRequestRefresh(QObject *object);
  void stopRequestRefresh(QObject *object);

  static CutieWindow *getCutieWindow(QApplication *application);
  static CutieWindow *getCutieWindow(QQmlEngine *engine);

  signals:
  void mouseMoveEventForward(QMouseEvent *event);
  void resizeEventForward(QResizeEvent *event);

  protected:
  std::vector<QObject *> _dependsOnRefresh = {};
};
