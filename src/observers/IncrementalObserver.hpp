#pragma once

#include <chrono>
#include <qproperty.h>

#include "Observer.hpp"

constexpr int wait_duration = 1000000000;

class IncrementalObserver : public Observer {
  public:
  QProperty<int> intensity{0};

  void init() override { REGISTER(intensity); }

  protected:
  void run() override {
    while (true) {
      intensity.setValue(intensity + 1);
      qDebug() << "IncrementalObserver: " << intensity;
      this->sleep(std::chrono::nanoseconds(wait_duration));
    }
  }
};