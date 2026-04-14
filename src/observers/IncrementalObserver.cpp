#include <chrono>

#include "IncrementalObserver.hpp"

void IncrementalObserver::init() { REGISTER(intensity); }

void IncrementalObserver::run() {
  while (true) {
    intensity.setValue(intensity + 1);
    qDebug() << "IncrementalObserver: " << intensity;
    this->sleep(std::chrono::nanoseconds(wait_duration));
  }
}