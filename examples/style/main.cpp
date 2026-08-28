#include "InNode.hpp"
#include "OutNode.hpp"

#include <NodeEditor/DataFlowContext>
#include <NodeEditor/DataFlowGraph>
#include <NodeEditor/NodeModelRegistry>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVariant>
#include <qjsondocument.h>
#include <qjsonobject.h>

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
  const auto styles = new StyleCollection();
  styles->followApplicationPalette(false);
  styles->setGraphicsViewStyle(GraphicsViewStyle((R"({
    "GraphicsViewStyle": {
      "BackgroundColor": [255, 255, 240],
      "FineGridColor": [245, 245, 230],
      "CoarseGridColor": [235, 235, 220]
    }
  })")));
  styles->setNodeStyle(NodeStyle(R"({
    "NodeStyle": {
      "NormalBoundaryColor": "darkgray",
      "SelectedBoundaryColor": "deepskyblue",
      "GradientColor0": "mintcream",
      "GradientColor1": "mintcream",
      "GradientColor2": "mintcream",
      "GradientColor3": "mintcream",
      "ShadowColor": [200, 200, 200],
      "ShadowEnabled": true,
      "FontColor": [10, 10, 10],
      "FontColorFaded": [100, 100, 100],
      "ConnectionPointColor": "white",
      "PenWidth": 2.0,
      "HoveredPenWidth": 2.5,
      "ConnectionPointDiameter": 10.0,
      "Opacity": 1.0
  }})"));
  styles->setConnectionStyle(ConnectionStyle(R"({
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": false
  }})"));

  context->setStyleCollection(styles);

  engine.setInitialProperties(QVariantMap({{"dataFlowContext", QVariant::fromValue(context)}}));
  engine.loadFromModule("examples.style", "Main");

  return app.exec();
}
