#include "Receiver.hpp"
#include "Orchestrator.hpp"
#include <qdebug.h>
#include <qlogging.h>

Receiver::Receiver(QObject *parent) : QObject(parent) {
  qDebug() << "newReceiver";

  auto engine = qmlEngine(this);
  qDebug() << "1";
  auto orchestrator = engine->singletonInstance<Orchestrator *>("Sandbox", "Orchestrator");
  qDebug() << "2";
  orchestrator->registerReceiver(this);
}

Receiver::~Receiver() {
  auto engine = qmlEngine(this);
  auto orchestrator = engine->singletonInstance<Orchestrator *>("Sandbox", "Orchestrator");
  orchestrator->unregisterReceiver(this);
}
