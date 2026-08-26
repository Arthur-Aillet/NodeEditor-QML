#include "AdditionNode.hpp"
#include "DisplayNode.hpp"
#include "DivisionNode.hpp"
#include "InputNode.hpp"
#include "MultiplicationNode.hpp"
#include "SubtractionNode.hpp"

#include "DataFlowContext.hpp"
#include "DataFlowGraph.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVariant>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("examples");
  QCoreApplication::setApplicationName("Calculator");
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  const auto reg = new NodeEditor::NodeModelRegistry(&engine);

  reg->registerModel<AdditionNode>("Operation");
  reg->registerModel<InputNode>("Input");
  reg->registerModel<DivisionNode>("Operation");
  reg->registerModel<DisplayNode>("Display");
  reg->registerModel<MultiplicationNode>("Operation");
  reg->registerModel<SubtractionNode>("Operation");

  const auto graph = new NodeEditor::DataFlowGraph(reg);
  const auto context = new NodeEditor::DataFlowContext(graph);

  engine.setInitialProperties(QVariantMap({{"dataFlowContext", QVariant::fromValue(context)}}));
  engine.loadFromModule("examples.calculator", "App");

  return app.exec();
}
