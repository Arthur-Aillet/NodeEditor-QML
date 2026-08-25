#include "AdditionNode.hpp"
#include "DataFlowContext.hpp"
#include "DisplayNode.hpp"
#include "DivisionNode.hpp"
#include "InputNode.hpp"
#include "MultiplicationNode.hpp"
#include "StyleCollection.hpp"
#include "SubtractionNode.hpp"

#include "DataFlowGraphModel.hpp"

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

  const auto reg = new NodeDelegateModelRegistry(&engine);

  reg->registerModel<AdditionNode>("Operation");
  reg->registerModel<InputNode>("Input");
  reg->registerModel<DivisionNode>("Operation");
  reg->registerModel<DisplayNode>("Display");
  reg->registerModel<MultiplicationNode>("Operation");
  reg->registerModel<SubtractionNode>("Operation");

  const auto graph = new DataFlowGraphModel(reg);
  const auto context = new DataFlowContext(graph);

  engine.setInitialProperties(QVariantMap({{"dataFlowContext", QVariant::fromValue(context)}}));
  engine.loadFromModule("examples.calculator", "App");

  return app.exec();
}