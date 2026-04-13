#pragma once

#include <QMetaProperty>
#include <QObject>

#include "Observer.hpp"
#include "Scene.hpp"

class Orchestrator : public QObject {
  Q_OBJECT
  private:
  std::vector<std::unique_ptr<Scene>> scenes;
  std::vector<std::shared_ptr<Observer>> observers;
  std::unordered_multimap<std::string, QMetaProperty> driver_mapping;
  std::shared_ptr<Observer> selectedObserver = nullptr;

  public slots:
  void getDriverValue(std::string name, QVariant value);

  public:
  void DeselectObserver();
  void SelectObserver(size_t index);
  Orchestrator(QObject *parent = nullptr);
};
