// Original implementation: https://github.com/vannell/qml_framerate
#pragma once

#include <QMetaObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTime>
#include <qelapsedtimer.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtimer.h>
#include <qtmetamacros.h>

class FrequencyMonitor : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
  Q_PROPERTY(
      int refreshPeriod READ refreshPeriod WRITE setRefreshPeriod NOTIFY refreshPeriodChanged)
  QML_ELEMENT

  public:
  FrequencyMonitor(QQuickItem *parent = nullptr);

  int fps() const;
  void setFps(int fps);

  // by default refresh one time per 1000 msec
  int refreshPeriod() const;
  void setRefreshPeriod(int msec);

  signals:
  void fpsChanged();
  void refreshPeriodChanged();

  public slots:
  void handleRefresh();
  void handleWindowChanged(QQuickWindow *window);

  protected:
  QMetaObject::Connection _windowConnection;
  QTimer _refreshTimer;
  int _fps;
  int _counter;
  int _refreshPeriod;
};
