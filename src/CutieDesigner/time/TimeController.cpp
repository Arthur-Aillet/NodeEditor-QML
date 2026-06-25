#include "TimeController.hpp"

TimeController::TimeController() : _currentPos(0), _minPos(0), _maxPos(15000), _playing(false) {}

TimeController::~TimeController() {
  if (_window != nullptr)
    _window->stopRequestRefresh(this);
}

TimeController *TimeController::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "TimeController has never been init.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return instance;
}

TimeController *TimeController::init() {
  instance = new TimeController();
  return instance;
};

void TimeController::linkCutieWindow(CutieWindow *window) {
  instance->_window = window;
  if (instance->_window != nullptr) {
    QObject::connect(instance->_window, SIGNAL(frameSwapped()), instance, SLOT(tickTime()));
    instance->_window->startRequestRefresh(instance);
    instance->_lastPoint = std::chrono::steady_clock::now();
  }
}

void TimeController::tickTime() {
  auto newPoint = std::chrono::steady_clock::now();
  if (_playing) {
    _currentPos += newPoint - _lastPoint;
    if (_currentPos >= _maxPos) {
      _playing = false;
      _currentPos = _maxPos;
      emit playingChanged();
    }
    emit currentPosChanged();
  }
  _lastPoint = newPoint;
}

double TimeController::getCurrentPos() { return _currentPos.count() / 1000; }
void TimeController::setCurrentPos(double val) {
  auto chrono = std::chrono::duration<double, std::milli>(val * 1000);
  if (chrono == _currentPos)
    return;
  _currentPos = chrono;
  emit currentPosChanged();
}

double TimeController::getMinPos() { return _minPos.count() / 1000; }
void TimeController::setMinPos(double val) {
  auto chrono = std::chrono::duration<double, std::milli>(val * 1000);
  if (chrono == _minPos)
    return;

  if (chrono > _maxPos) {
    _minPos = _maxPos;
  } else {
    _minPos = chrono;
  }
  emit minPosChanged();
  if (_currentPos < _minPos)
    setCurrentPos(_minPos.count() / 1000);
}

double TimeController::getMaxPos() { return _maxPos.count() / 1000; }
void TimeController::setMaxPos(double val) {
  auto chrono = std::chrono::duration<double, std::milli>(val * 1000);
  if (chrono == _maxPos)
    return;

  if (chrono < _minPos) {
    _maxPos = _minPos;
  } else {
    _maxPos = chrono;
  }
  emit maxPosChanged();
  if (_currentPos > _maxPos)
    setCurrentPos(_maxPos.count() / 1000);
}

bool TimeController::getPlaying() { return _playing; }
void TimeController::setPlaying(bool val) {
  _playing = val;
  emit playingChanged();
}

void TimeController::stop() {
  setPlaying(false);
  setCurrentPos(_minPos.count() / 1000);
}