#include "TimeController.hpp"

#include <QtConcurrentTask>
#include <optional>
#include <qimage.h>
#include <qstandardpaths.h>
#include <qtypes.h>
#include <vector>

TimeController::TimeController()
    : _currentTime(0.0), _minFrame(0), _maxFrame(900), _currentFrame(0), _playing(false),
      _frames{std::vector<std::optional<QImage>>(_maxFrame - _minFrame, std::optional<QImage>())} {}

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
    QObject::connect(instance->_window, SIGNAL(frameSwapped()), instance, SLOT(frameSwapped()));
    if (instance->_playing) {
      instance->_window->startRequestRefresh(instance);
      instance->_lastPoint = std::chrono::steady_clock::now();
    }
  }
}

void TimeController::frameSwapped() {
  auto newPoint = std::chrono::steady_clock::now();
  if (_playing) {
    if (_recording) {
      QImage capture = _window->grabWindow();
      _frames[_currentFrame - _minFrame] = capture;
    }

    _currentFrame += 1;
    _currentTime += newPoint - _lastPoint;

    if (_currentFrame >= _maxFrame) {
      if (_looping) {
        _currentFrame = _minFrame;
        _currentTime = std::chrono::duration<double>(_currentFrame / 60.0);
      } else {
        _playing = false;
        emit playingChanged();
      }
    }
    emit currentTimeChanged();
    emit currentFrameChanged();
  }
  _lastPoint = newPoint;
}

double TimeController::currentTime() { return _currentTime.count(); }

void TimeController::setCurrentFrame(uint newCurrentFrame) {
  if (_currentFrame == newCurrentFrame)
    return;
  _currentFrame = std::clamp(newCurrentFrame, _minFrame, _maxFrame);
  _currentTime = std::chrono::duration<double>(_currentFrame / 60.0);
  emit currentTimeChanged();
  emit currentFrameChanged();
}

uint TimeController::minFrame() { return _minFrame; }

void TimeController::setMinFrame(uint newMinFrame) {
  if (_minFrame == newMinFrame || (newMinFrame > _maxFrame && _minFrame == _maxFrame))
    return;

  if (newMinFrame > _maxFrame) {
    _minFrame = _maxFrame;
  } else {
    _minFrame = newMinFrame;
  }
  _frames.resize(_maxFrame - _minFrame, std::optional<QImage>());
  emit minFrameChanged();
  if (_currentFrame < _minFrame)
    setCurrentFrame(_minFrame);
}

uint TimeController::maxFrame() { return _maxFrame; }
void TimeController::setMaxFrame(uint newMaxFrame) {
  if (_maxFrame == newMaxFrame || (newMaxFrame < _minFrame && _maxFrame == _minFrame))
    return;

  if (newMaxFrame < _minFrame) {
    _maxFrame = _minFrame;
  } else {
    _maxFrame = newMaxFrame;
  }
  _frames.resize(_maxFrame - _minFrame, std::optional<QImage>());
  emit maxFrameChanged();
  if (_currentFrame > _maxFrame)
    setCurrentFrame(_maxFrame);
}

bool TimeController::playing() { return _playing; }
void TimeController::setPlaying(bool playing) {
  _playing = playing;
  if (playing) {
    _lastPoint = std::chrono::steady_clock::now();
    _window->startRequestRefresh(this);
  } else {
    _window->stopRequestRefresh(this);
  }
  emit playingChanged();
}

void TimeController::stop() {
  setPlaying(false);
  setCurrentFrame(_minFrame);
}

bool TimeController::looping() { return _looping; }

void TimeController::setLooping(bool looping) {
  if (_looping == looping)
    return;
  _looping = looping;
  emit loopingChanged();
}

bool TimeController::recording() { return _recording; }
void TimeController::setRecording(bool recording) {
  if (_recording == recording)
    return;
  if (recording == false) {
    auto task = [](std::vector<std::optional<QImage>> frames) {
      for (int i = 0; i != frames.size(); i++) {
        if (frames[i].has_value()) {
          QString path = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0];
          path.append(QString().asprintf("/Capture/image%04d.png", i));
          frames[i].value().save(path, nullptr, 100);
        }
      }
    };
    auto future = QtConcurrent::task(std::move(task)).withArguments(_frames).spawn();
  }
  _frames.assign(_maxFrame - _minFrame, std::optional<QImage>());
  _recording = recording;
  emit recordingChanged();
}

uint TimeController::currentFrame() { return _currentFrame; }