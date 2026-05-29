#include "DataFlowModelInterface.hpp"

#include "AdditionModel.hpp"
#include "Definitions.hpp"
#include "DivisionModel.hpp"
#include "MultiplicationModel.hpp"
#include "Object/ATypeNode.hpp"
#include "Object/ObjectDisplayDataModel.hpp"
#include "ObjectLoader.hpp"
#include "SubtractionModel.hpp"
#include "TextDisplayDataModel.hpp"
#include "TextTyperModel.hpp"
#include "ValueNodeModel.hpp"

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <memory>
#include <qboxlayout.h>
#include <qlogging.h>
#include <qobjectdefs.h>
#include <qqml.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qwidget.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  auto ret = std::make_shared<NodeDelegateModelRegistry>(&engine);

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionModel>("Process");
  ret->registerModel<DivisionModel>("Process");
  ret->registerModel<MultiplicationModel>("Process");
  ret->registerModel<SubtractionModel>("Process");
  ret->registerModel<TextDisplayDataModel>("Display");
  ret->registerModel<ObjectDisplayDataModel>("Display");
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

  auto source = model.addNode(ObjectDisplayDataModel(&engine).name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 0));
  model.setNodeData(source, NodeRole::Type, ObjectDisplayDataModel(&engine).name());
  auto display = model.delegateModel<ObjectDisplayDataModel>(source);

  auto loader = item->property("objectLoader").value<ObjectLoader *>();
  loader->connectFinalNode(display);

  auto text = model.addNode(TextDisplayDataModel(&engine).name());
  model.setNodeData(text, NodeRole::Position, QPointF(400, 200));
  model.setNodeData(text, NodeRole::Type, TextDisplayDataModel(&engine).name());
  auto displayText = model.delegateModel<TextDisplayDataModel>(text);
  QObject::connect(displayText, SIGNAL(valueUpdated(QString)), item, SIGNAL(valueUpdated(QString)));

  return app.exec();
}