#include "DataFlowModelInterface.hpp"

#include "AdditionModel.hpp"
#include "Definitions.hpp"
#include "DivisionModel.hpp"
#include "MultiplicationModel.hpp"
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

  auto ret = std::make_shared<NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionModel>("Process");
  ret->registerModel<DivisionModel>("Process");
  ret->registerModel<MultiplicationModel>("Process");
  ret->registerModel<SubtractionModel>("Process");
  ret->registerModel<TextDisplayDataModel>("Display");
  ret->registerModel<TextTyperModel>("Process");

  QQmlApplicationEngine engine;

  auto model = DataFlowGraphModel(ret, &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesignerModule", "Main");
  {
    auto source = model.addNode(ValueNodeModel().name());
    model.setNodeData(source, NodeRole::Position, QPointF(0, 0));
    model.setNodeData(source, NodeRole::Type, ValueNodeModel().name());
  }

  {
    auto source = model.addNode(TextTyperModel().name());
    model.setNodeData(source, NodeRole::Position, QPointF(100, 100));
    model.setNodeData(source, NodeRole::Type, TextTyperModel().name());
  }

  QObject *item = engine.rootObjects().first();

  auto source = model.addNode(TextDisplayDataModel().name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 0));
  model.setNodeData(source, NodeRole::Type, TextDisplayDataModel().name());
  auto display = model.delegateModel<TextDisplayDataModel>(source);
  QObject::connect(display, SIGNAL(valueUpdated(QString)), item, SIGNAL(newDisplayValue(QString)));

  return app.exec();
}