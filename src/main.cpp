#include "DataFlowModelInterface.hpp"

#include "ATypeNode.hpp"
#include "AdditionModel.hpp"
#include "Definitions.hpp"
#include "DivisionModel.hpp"
#include "MultiplicationModel.hpp"
#include "SubtractionModel.hpp"
#include "SurfaceDisplayDataModel.hpp"
#include "SurfaceLoader.hpp"
#include "TextTyperModel.hpp"
#include "ValueNodeModel.hpp"

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

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionModel>("Process");
  ret->registerModel<DivisionModel>("Process");
  ret->registerModel<MultiplicationModel>("Process");
  ret->registerModel<SubtractionModel>("Process");
  ret->registerModel<SurfaceDisplayDataModel>("Display");
  ret->registerModel<ATypeNode>("Display");
  ret->registerModel<TextTyperModel>("Process");

  auto model = DataFlowGraphModel(ret, &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesignerModule", "Main");
  {
    auto source = model.addNode(ValueNodeModel(&engine).name());
    model.setNodeData(source, NodeRole::Position, QPointF(0, 0));
    model.setNodeData(source, NodeRole::Type, ValueNodeModel(&engine).name());
  }

  {
    auto source = model.addNode(TextTyperModel(&engine).name());
    model.setNodeData(source, NodeRole::Position, QPointF(100, 100));
    model.setNodeData(source, NodeRole::Type, TextTyperModel(&engine).name());
  }

  QObject *item = engine.rootObjects().first();

  auto source = model.addNode(SurfaceDisplayDataModel(&engine).name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 150));
  model.setNodeData(source, NodeRole::Type, SurfaceDisplayDataModel(&engine).name());
  model.setNodeData(source, NodeRole::Flags, NodeFlag::Locked);
  auto display = model.delegateModel<SurfaceDisplayDataModel>(source);

  auto loader = item->property("objectLoader").value<SurfaceLoader *>();
  loader->connectFinalNode(display);

  return app.exec();
}