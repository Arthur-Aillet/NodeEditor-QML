#include "SimpleGraphModel.hpp"
#include <QApplication>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // 1. Create the graph model
  SimpleGraphModel model;

  // 2. Create a scene that visualizes the model
  auto *scene = new QtNodes::BasicGraphicsScene(model); // NOLINT

  // 3. Create a view to display the scene
  QtNodes::GraphicsView view(scene);
  view.setWindowTitle("My First Node Graph");
  view.resize(800, 600); // NOLINT
  view.show();

  return app.exec();
}