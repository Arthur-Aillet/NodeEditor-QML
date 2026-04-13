#pragma once

#include <QMetaProperty>
#include <QObject>

#include "Observer.hpp"
#include "Scene.hpp"
#include "observers/IncrementalObserver.hpp"
#include "scenes/ColorScene.hpp"

class Orchestrator : public QObject {
  Q_OBJECT
  std::vector<std::unique_ptr<Scene>> scenes;
  std::vector<std::unique_ptr<Observer>> observers;
  std::unordered_multimap<std::string, QMetaProperty> driver_mapping;

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
  Orchestrator(QObject *parent) : QObject(parent) {
    observers.push_back(std::make_unique<IncrementalObserver>());
    observers[0]->start();

    scenes.push_back(std::make_unique<ColorScene>(this));
    scenes[0]->display_value();

    for (auto &scene : scenes) {
      const QMetaObject *metaObject = scene->metaObject();

      for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        driver_mapping.insert(std::pair("intensity", metaObject->property(i)));
      }
    }

    QObject::connect(&*observers[0], SIGNAL(setDriverValue(std::string, QVariant)), this,
                     SLOT(getDriverValue(std::string, QVariant)));
  }
};
