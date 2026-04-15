#include "Orchestrator.hpp"
#include "IncrementalObserver.hpp"
#include "Receiver.hpp"
#include <iostream>
#include <memory>
#include <qlogging.h>
#include <qobject.h>
#include <qvariant.h>

Orchestrator::Orchestrator(QObject *parent) : QObject(parent) {
  std::cout << "Orchestrator Constructor" << std::endl;
  observers.push_back(std::make_shared<IncrementalObserver>());

  observers[0]->init();

  SelectObserver(0);
}

void Orchestrator::registerReceiver(Receiver *receiver) { active_receivers.push_back(receiver); }

void Orchestrator::unregisterReceiver(Receiver *receiver) {
  active_receivers.erase(std::remove(active_receivers.begin(), active_receivers.end(), receiver),
                         active_receivers.end());
}

void Orchestrator::DeselectObserver() {
  if (selected_observer != nullptr) {
    selected_observer->exit();
    selected_observer->disconnect(SIGNAL(sendDriverValue(std::string, QVariant)), this,
                                  SLOT(applyDriverValue(std::string, QVariant)));
  }
}

void Orchestrator::applyDriverValue(std::string name, QVariant value) {
  for (auto receiver : active_receivers) {
    auto range = receiver->driver_mapping.equal_range(name);

    for (auto it = range.first; it != range.second; ++it) {
      auto [_, property_index] = *it;
      auto property_name = receiver->metaObject()->property(property_index).name();
      receiver->setProperty(property_name, value);
    }
  }
}

void Orchestrator::SelectObserver(size_t index) {
  if (index < observers.size() && observers[index].unique()) {
    DeselectObserver();

    selected_observer = observers[index];
    selected_observer->start();
    QObject::connect(&*selected_observer, SIGNAL(sendDriverValue(std::string, QVariant)), this,
                     SLOT(applyDriverValue(std::string, QVariant)));
  }
}
