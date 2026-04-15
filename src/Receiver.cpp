#include "Receiver.hpp"
#include "EngineAccess.hpp"
#include "Orchestrator.hpp"
#include <qdebug.h>
#include <qlogging.h>
#include <qqml.h>
#include <qqmlengine.h>

Receiver::Receiver(QObject *parent) : QObject(parent) {

  auto orchestrator =
      EngineAccess::engine->singletonInstance<Orchestrator *>("Sandbox", "Orchestrator");
  orchestrator->registerReceiver(this);
}

Receiver::~Receiver() {
  qDebug() << "destroyed";

  auto orchestrator =
      EngineAccess::engine->singletonInstance<Orchestrator *>("Sandbox", "Orchestrator");
  orchestrator->unregisterReceiver(this);
}
