#include "main.hpp"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcomponent.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  qDebug() << "Launching Sandbox!";
  QQmlApplicationEngine engine;
  SceneManager sceneManager = SceneManager(&engine);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("Sandbox", "Main");

  return QCoreApplication::exec();
}
