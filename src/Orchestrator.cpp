#include "Orchestrator.hpp"
#include "ColorScene.hpp"
#include "IncrementalObserver.hpp"

Orchestrator::Orchestrator(QObject *parent) : QObject(parent) {
  observers.push_back(std::make_shared<IncrementalObserver>());
  observers[0]->init();
  scenes.push_back(std::make_unique<ColorScene>(this));

  for (auto &scene : scenes) {
    auto metaObject = scene->metaObject();

    for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
      driver_mapping.insert(std::pair("intensity", metaObject->property(i)));
    }
  }
  SelectObserver(0);
}

void Orchestrator::getDriverValue(std::string name, QVariant value) {
  auto range = driver_mapping.equal_range(name);

  for (auto it = range.first; it != range.second; ++it) {
    auto [_, scene_property] = *it;

    scenes[0]->setProperty(scene_property.name(), value);
  }
}

void Orchestrator::DeselectObserver() {
  if (selectedObserver != nullptr) {
    selectedObserver->exit();
    selectedObserver->disconnect(SIGNAL(setDriverValue(std::string, QVariant)), this,
                                 SLOT(getDriverValue(std::string, QVariant)));
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
