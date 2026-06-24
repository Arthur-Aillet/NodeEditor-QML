#include "FrequencyMonitor.hpp"
#include <QQuickWindow>
#include <QTime>

FrequencyMonitor::FrequencyMonitor(QQuickItem *parent)
    : QQuickItem(parent), _counter(0), _refreshPeriod(1000) {
  connect(this, &FrequencyMonitor::windowChanged, this, &FrequencyMonitor::handleWindowChanged);
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
  emit refreshPeriodChanged();
}

void FrequencyMonitor::handleAfterRendering() {
  const int elapsedMsec = _timer.elapsed();

  _counter++;

  if (elapsedMsec >= _refreshPeriod) {
    setFps(_counter / (elapsedMsec / 1000.0));

    _counter = 0;
    _timer.restart();
  }
}

void FrequencyMonitor::handleWindowChanged(QQuickWindow *window) {
  if (_windowConnection)
    this->disconnect(_windowConnection);

  if (window) {
    _timer.restart();
    _windowConnection = connect(window, &QQuickWindow::afterRendering, this,
                                &FrequencyMonitor::handleAfterRendering);
  }
}