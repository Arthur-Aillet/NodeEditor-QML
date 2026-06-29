#pragma once

#include "CutieWindow.hpp"

#include <chrono>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <ratio>

class TimeController : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  Q_PROPERTY(double currentPos READ getCurrentPos WRITE setCurrentPos NOTIFY currentPosChanged)
  Q_PROPERTY(double minPos READ getMinPos WRITE setMinPos NOTIFY minPosChanged)
  Q_PROPERTY(double maxPos READ getMaxPos WRITE setMaxPos NOTIFY maxPosChanged)
  Q_PROPERTY(bool playing READ getPlaying WRITE setPlaying NOTIFY playingChanged)

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

  double getCurrentPos();
  void setCurrentPos(double val);
  double getMinPos();
  void setMinPos(double val);
  double getMaxPos();
  void setMaxPos(double val);
  bool getPlaying();
  void setPlaying(bool val);

  signals:
  void currentPosChanged();
  void minPosChanged();
  void maxPosChanged();
  void playingChanged();

  public slots:
  void tickTime();

  private:
  std::chrono::duration<double, std::milli> _currentPos;
  std::chrono::duration<double, std::milli> _minPos;
  std::chrono::duration<double, std::milli> _maxPos;
  std::chrono::time_point<std::chrono::steady_clock> _lastPoint;
  bool _playing = false;
  CutieWindow *_window = nullptr;
};