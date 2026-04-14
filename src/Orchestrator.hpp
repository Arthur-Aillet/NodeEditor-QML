#pragma once

#include "Observer.hpp"
#include "Receiver.hpp"
#include <QObject>
#include <memory>
#include <qdebug.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <string>
#include <vector>

class Orchestrator : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  private:
  std::vector<std::shared_ptr<Observer>> observers;
  std::vector<Receiver *> active_receivers;
  std::shared_ptr<Observer> selectedObserver = nullptr;

  public slots:
  void getDriverValue(std::string name, QVariant value);

  public:
  void registerReceiver(Receiver *receiver);
  void unregisterReceiver(Receiver *receiver);

  Orchestrator(QObject *parent = nullptr);

  private:
  void DeselectObserver();
  void SelectObserver(size_t index);
};
