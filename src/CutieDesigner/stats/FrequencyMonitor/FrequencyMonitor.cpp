#include "FrequencyMonitor.hpp"

#include <QQuickWindow>
#include <QTime>

FrequencyMonitor::FrequencyMonitor(QQuickItem *parent)
    : QQuickItem(parent), _counter(0), _refreshPeriod(1000), _refreshTimer() {
  connect(this, &FrequencyMonitor::windowChanged, this, &FrequencyMonitor::handleWindowChanged);
  connect(&_refreshTimer, &QTimer::timeout, this, &FrequencyMonitor::handleRefresh);
  _refreshTimer.start(_refreshPeriod);
  _refreshTimer.setTimerType(Qt::PreciseTimer);
}

int FrequencyMonitor::fps() const { return _fps; }

void FrequencyMonitor::setFps(int fps) {
  if (_fps == fps)
    return;

  _fps = fps;
  emit fpsChanged();
}

int FrequencyMonitor::refreshPeriod() const { return _refreshPeriod; }

void FrequencyMonitor::setRefreshPeriod(int msec) {
  if (_refreshPeriod == msec)
    return;

  _refreshPeriod = msec;
  _refreshTimer.start(_refreshPeriod);
  emit refreshPeriodChanged();
}

void FrequencyMonitor::handleRefresh() {
  setFps(_counter / ((float)_refreshPeriod / 1000.0));
  _counter = 0;
  if (window()) {
    window()->requestUpdate();
  }
}

void FrequencyMonitor::handleWindowChanged(QQuickWindow *window) {
  if (_windowConnection)
    this->disconnect(_windowConnection);

  if (window) {
    _refreshTimer.start(_refreshPeriod);
    _windowConnection = connect(window, &QQuickWindow::afterRendering, [this]() { _counter++; });
  }
}