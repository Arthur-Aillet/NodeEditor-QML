#pragma once

#include <QMetaProperty>
#include <QObject>
#include <cstddef>
#include <memory>

#include "Observer.hpp"
#include "Scene.hpp"
#include "observers/IncrementalObserver.hpp"
#include "scenes/ColorScene.hpp"

class Orchestrator : public QObject {
  Q_OBJECT
  std::vector<std::unique_ptr<Scene>> scenes;
  std::vector<std::shared_ptr<Observer>> observers;
  std::unordered_multimap<std::string, QMetaProperty> driver_mapping;
  std::shared_ptr<Observer> selectedObserver = nullptr;

  public slots:
  void getDriverValue(std::string name, QVariant value) {
    auto range = driver_mapping.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
      auto [observerKey, scene_property] = *it;
      auto a = scenes[0]->property(scene_property.name()).toInt();
      qDebug() << "YES!" << observerKey << "  " << scene_property.name() << " : " << value << "  "
               << a;
      scenes[0]->setProperty(scene_property.name(), value);
    }
  }

  public:
  void DeselectObserver() {
    if (selectedObserver != nullptr) {
      selectedObserver->exit();
      selectedObserver->disconnect(SIGNAL(setDriverValue(std::string, QVariant)), this,
                                   SLOT(getDriverValue(std::string, QVariant)));
    }
  }

  void SelectObserver(size_t index) {
    if (index < observers.size() && observers[index].unique()) {
      DeselectObserver();

      selectedObserver = observers[index];

      selectedObserver->start();
      QObject::connect(&*selectedObserver, SIGNAL(setDriverValue(std::string, QVariant)), this,
                       SLOT(getDriverValue(std::string, QVariant)));
    }
  }

  Orchestrator(QObject *parent) : QObject(parent) {
    observers.push_back(std::make_shared<IncrementalObserver>());

    scenes.push_back(std::make_unique<ColorScene>(this));

    for (auto &scene : scenes) {
      const QMetaObject *metaObject = scene->metaObject();

      for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        driver_mapping.insert(std::pair("intensity", metaObject->property(i)));
      }
    }
    SelectObserver(0);
  }
};
