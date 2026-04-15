#include "GraphModel.hpp"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("CutieDesignerModule", "Main");

  return QCoreApplication::exec();

  GraphModel model;

  auto *scene = new QtNodes::BasicGraphicsScene(model); // NOLINT

  QtNodes::GraphicsView view(scene);
  view.setWindowTitle("My First Node Graph");
  view.resize(800, 600); // NOLINT
  view.show();

  view.setContextMenuPolicy(Qt::ActionsContextMenu);

  auto *createAction = new QAction("Create Node", &view);
  QObject::connect(createAction, &QAction::triggered, [&]() {
    // Get mouse position in scene coordinates
    QPointF pos = view.mapToScene(view.mapFromGlobal(QCursor::pos()));

    // Add node to model
    auto nodeId = model.addNode();
    model.setNodeData(nodeId, QtNodes::NodeRole::Position, pos);
  });

  view.addAction(createAction);

  return app.exec();
}