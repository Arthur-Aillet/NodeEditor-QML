#include "Orchestrator.hpp"
#include "IncrementalObserver.hpp"
#include "Receiver.hpp"
#include <memory>
#include <qlogging.h>
#include <qobject.h>
#include <qvariant.h>

Orchestrator::Orchestrator(QObject *parent) : QObject(parent) {
  QJSEngine::setObjectOwnership(parent, QJSEngine::CppOwnership);
  qDebug() << "Test?";
  observers.push_back(std::make_shared<IncrementalObserver>());
  qDebug() << "2";

  observers[0]->init();

  SelectObserver(0);
}

void Orchestrator::registerReceiver(Receiver *receiver) {
  qDebug() << "reg";

  active_receivers.push_back(receiver);
}

void Orchestrator::unregisterReceiver(Receiver *receiver) {
  active_receivers.erase(std::remove(active_receivers.begin(), active_receivers.end(), receiver),
                         active_receivers.end());
}

void Orchestrator::DeselectObserver() {
  if (selectedObserver != nullptr) {
    selectedObserver->exit();
    selectedObserver->disconnect(SIGNAL(setDriverValue(std::string, QVariant)), this,
                                 SLOT(getDriverValue(std::string, QVariant)));
  }
}

void Orchestrator::getDriverValue(std::string name, QVariant value) {
  for (auto receiver : active_receivers) {
    auto range = receiver->driver_mapping.equal_range(name);

    for (auto it = range.first; it != range.second; ++it) {
      auto [_, scene_property] = *it;

      receiver->setProperty(scene_property.name(), value);
    }
  }
}

void Orchestrator::SelectObserver(size_t index) {
  if (index < observers.size() && observers[index].unique()) {
    DeselectObserver();

    selectedObserver = observers[index];
    selectedObserver->start();
    QObject::connect(&*selectedObserver, SIGNAL(setDriverValue(std::string, QVariant)), this,
                     SLOT(getDriverValue(std::string, QVariant)));
  }
}
