#pragma once

#include <qproperty.h>

#include "Observer.hpp"

constexpr int wait_duration = 1000000000;

class IncrementalObserver : public Observer {
  public:
  QProperty<int> intensity{0};

  IncrementalObserver() = default;

  void init() override;

  protected:
  void run() override;
};