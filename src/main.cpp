#include "EngineAccess.hpp"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcomponent.h>
#include <qurl.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  EngineAccess access;
  qDebug() << "Launching Sandbox!" << access.engine->baseUrl();

  QObject::connect(
      &*access.engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  access.engine->loadFromModule("Sandbox", "Main");

  return QCoreApplication::exec();
}
