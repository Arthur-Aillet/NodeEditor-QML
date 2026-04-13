#pragma once

#include "src/Observer.hpp"
#include <chrono>

constexpr int wait_duration = 100000000;

class IncrementalObserver : public Observer {
  public:
  int intensity = 0;

  protected:
  void run() override {
    while (true) {
      intensity += 1;
      UPDATE_VALUE(intensity);
      qDebug() << "IncrementalObserver: " << intensity;
      this->sleep(std::chrono::nanoseconds(wait_duration));
    }
  }
};