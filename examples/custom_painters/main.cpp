#include "InNode.hpp"
#include "OutNode.hpp"

#include <NodeEditor/DataFlowContext>
#include <NodeEditor/DataFlowGraph>
#include <NodeEditor/NodeModelRegistry>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QVariant>
#include <memory>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("examples");
  QCoreApplication::setApplicationName("Calculator");
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  const auto reg = new NodeEditor::NodeModelRegistry(&engine);

  reg->registerModel<InNode>("Nodes");
  reg->registerModel<OutNode>("Nodes");

  const auto graph = new NodeEditor::DataFlowGraph(reg);
  const auto context = new NodeEditor::DataFlowContext(graph);

  context->setNodePainter(
      std::make_unique<QQmlComponent>(&engine, "examples.custom_painters", "CustomNodePainter"));

  engine.setInitialProperties(QVariantMap({{"dataFlowContext", QVariant::fromValue(context)}}));
  engine.loadFromModule("examples.custom_painters", "Main");

  return app.exec();
}
