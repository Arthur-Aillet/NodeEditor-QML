#include "ATypeNode.hpp"
#include "AdditionNode.hpp"
#include "DataFlowModelInterface.hpp"
#include "Definitions.hpp"
#include "DivisionNode.hpp"
#include "MultiplicationNode.hpp"
#include "NumberInputNode.hpp"
#include "SubtractionNode.hpp"
#include "SurfaceDisplayNode.hpp"
#include "SurfaceLoader.hpp"
#include "TextTyperNode.hpp"
#include "UkrugNode.hpp"

#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtWidgets/QApplication>
#include <memory>
#include <qdebug.h>
#include <qquickitem.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  auto ret = std::make_shared<NodeDelegateModelRegistry>(&engine);

  ret->registerModel<NumberInputNode>("Input");
  ret->registerModel<AdditionNode>("Process");
  ret->registerModel<DivisionNode>("Process");
  ret->registerModel<MultiplicationNode>("Process");
  ret->registerModel<SubtractionNode>("Process");
  ret->registerModel<SurfaceDisplayNode>("Display");
  ret->registerModel<UkrugNode>("Display");
  ret->registerModel<ATypeNode>("Display");
  ret->registerModel<TextTyperNode>("Process");

  auto model = DataFlowGraphModel(ret, &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesignerModule", "Main");
  QObject *item = engine.rootObjects().first();

  auto source = model.addNode(SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 150));
  model.setNodeData(source, NodeRole::Type, SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Flags, NodeFlag::Locked);
  auto display = model.delegateModel<SurfaceDisplayNode>(source);

  auto loader = item->property("objectLoader").value<SurfaceLoader *>();
  loader->connectFinalNode(display);

  auto id1 = model.addNode(ATypeNode(&engine).name());
  model.setNodeData(id1, NodeRole::Position, QPointF(220, 40));
  model.setNodeData(id1, NodeRole::Type, ATypeNode(&engine).name());

  auto id2 = model.addNode(TextTyperNode(&engine).name());
  model.setNodeData(id2, NodeRole::Position, QPointF(0, 0));
  model.setNodeData(id2, NodeRole::Type, TextTyperNode(&engine).name());

  auto ukr = model.addNode(UkrugNode(&engine).name());
  model.setNodeData(ukr, NodeRole::Position, QPointF(100, 150));
  model.setNodeData(ukr, NodeRole::Type, UkrugNode(&engine).name());

  return app.exec();
}