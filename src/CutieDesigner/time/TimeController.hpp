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
  Q_PROPERTY(
      uint currentFrame READ getCurrentFrame WRITE setCurrentFrame NOTIFY currentFrameChanged)
  Q_PROPERTY(uint minFrame READ getMinFrame WRITE setMinFrame NOTIFY minFrameChanged)
  Q_PROPERTY(uint maxFrame READ getMaxFrame WRITE setMaxFrame NOTIFY maxFrameChanged)
  Q_PROPERTY(double currentTime READ getCurrentTime NOTIFY currentTimeChanged)
  Q_PROPERTY(bool playing READ getPlaying WRITE setPlaying NOTIFY playingChanged)
  Q_PROPERTY(bool looping READ getLooping WRITE setLooping NOTIFY loopingChanged)

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

  uint getCurrentFrame();
  void setCurrentFrame(uint currentFrame);
  uint getMinFrame();
  void setMinFrame(uint minFrame);
  uint getMaxFrame();
  void setMaxFrame(uint maxFrame);
  double getCurrentTime();

  bool getPlaying();
  void setPlaying(bool val);
  bool getLooping();
  void setLooping(bool val);

  signals:
  void currentFrameChanged();
  void minFrameChanged();
  void maxFrameChanged();
  void currentTimeChanged();
  void playingChanged();
  void loopingChanged();

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
  CutieWindow *_window = nullptr;
};