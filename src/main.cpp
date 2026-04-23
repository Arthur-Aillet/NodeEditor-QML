#include "DataFlowModelInterface.hpp"
#include "NodeData.hpp"
#include "NodeModel.hpp"
#include "QtNodes/internal/BasicGraphicsScene.hpp"
#include "QtNodes/internal/DataFlowGraphModel.hpp"
#include "QtNodes/internal/DataFlowGraphicsScene.hpp"
#include "ValueNodeModel.hpp"
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <memory>
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
  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<ValueNodeModel1>("Input2");
  ret->registerModel<ValueNodeModel2>("Input3");
  ret->registerModel<ValueNodeModel3>("Input4");
  ret->registerModel<ValueNodeModel5>("Input5");
  ret->registerModel<ValueNodeModel4>("Input6");
  ret->registerModel<AdditionNode>("Process");

  auto model = QtNodes::DataFlowGraphModel(ret);
  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesignerModule", "Main");

  {
    auto source = model.addNode(ValueNodeModel().name());
    model.setNodeData(source, NodeRole::Position, QPointF(0, 0));
    model.setNodeData(source, NodeRole::Type, ValueNodeModel().name());
  }
  return app.exec();
}