#include "AdditionNode.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "DataFlowGraphModel.hpp"
#include "DataFlowModelInterface.hpp"

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

  std::shared_ptr<DataFlowGraphModel> graph = std::make_shared<DataFlowGraphModel>(reg, &engine);

  DataFlowModelInterface::init(graph.get());

  engine.loadFromModule("examples.calculator", "App");

  return app.exec();
}