#pragma once

#include "CutieWindow.hpp"

#include <chrono>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class TimeController : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  Q_PROPERTY(uint currentFrame READ currentFrame WRITE setCurrentFrame NOTIFY currentFrameChanged)
  Q_PROPERTY(uint minFrame READ minFrame WRITE setMinFrame NOTIFY minFrameChanged)
  Q_PROPERTY(uint maxFrame READ maxFrame WRITE setMaxFrame NOTIFY maxFrameChanged)
  Q_PROPERTY(double currentTime READ currentTime NOTIFY currentTimeChanged)
  Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
  Q_PROPERTY(bool looping READ looping WRITE setLooping NOTIFY loopingChanged)
  Q_PROPERTY(bool recording READ recording WRITE setRecording NOTIFY recordingChanged)

  static TimeController *create(QQmlEngine *, QJSEngine *engine);
  static TimeController *init();

  protected:
  inline static TimeController *instance = nullptr;
  TimeController();
  ~TimeController();

  public:
  static void linkCutieWindow(CutieWindow *window);
  static TimeController *get() { return instance; }

  Q_INVOKABLE void stop();

  uint currentFrame();
  void setCurrentFrame(uint currentFrame);
  uint minFrame();
  void setMinFrame(uint minFrame);
  uint maxFrame();
  void setMaxFrame(uint maxFrame);
  double currentTime();

  bool playing();
  void setPlaying(bool playing);
  bool looping();
  void setLooping(bool looping);
  bool recording();
  void setRecording(bool recording);

  signals:
  void currentFrameChanged();
  void minFrameChanged();
  void maxFrameChanged();
  void currentTimeChanged();
  void playingChanged();
  void loopingChanged();
  void recordingChanged();

  public slots:
  void frameSwapped();

  private:
  std::chrono::duration<double> _currentTime;
  std::chrono::time_point<std::chrono::steady_clock> _lastPoint;
  uint _currentFrame;
  uint _minFrame;
  uint _maxFrame;
  bool _playing = false;
  bool _looping = true;
  bool _recording = false;
  std::vector<std::optional<QImage>> _frames;
  CutieWindow *_window = nullptr;
};