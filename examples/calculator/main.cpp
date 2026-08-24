#include "AdditionNode.hpp"
#include "DisplayNode.hpp"
#include "DivisionNode.hpp"
#include "InputNode.hpp"
#include "MultiplicationNode.hpp"
#include "SubtractionNode.hpp"

#include "DataFlowGraphModel.hpp"
#include "DataFlowModelInterface.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("examples");
  QCoreApplication::setApplicationName("Calculator");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  auto reg = std::make_shared<NodeDelegateModelRegistry>(&engine);

  reg->registerModel<AdditionNode>("Operation");
  reg->registerModel<InputNode>("Input");
  reg->registerModel<DivisionNode>("Operation");
  reg->registerModel<DisplayNode>("Display");
  reg->registerModel<MultiplicationNode>("Operation");
  reg->registerModel<SubtractionNode>("Operation");

  std::shared_ptr<DataFlowGraphModel> graph = std::make_shared<DataFlowGraphModel>(reg, &engine);

  DataFlowModelInterface::init(graph.get());

  engine.loadFromModule("examples.calculator", "App");

  return app.exec();
}