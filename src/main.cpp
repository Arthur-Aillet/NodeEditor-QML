#include "NodeEditor.hpp"
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <qqmlapplicationengine.h>

using QtNodes::BasicGraphicsScene;
using QtNodes::ConnectionStyle;
using QtNodes::GraphicsView;
using QtNodes::NodeRole;
using QtNodes::StyleCollection;

int main(int argc, char *argv[]) {
  qputenv("QT_QUICK_BACKEND", "software");
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.addImportPath(":/");
  engine.loadFromModule("CutieDesignerModule", "Main");

  return app.exec();
}