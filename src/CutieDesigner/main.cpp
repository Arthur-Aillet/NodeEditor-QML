#include "CutieWindow.hpp"
#include "DataFlowGraphModel.hpp"
#include "DataFlowModelInterface.hpp"
#include "Definitions.hpp"
#include "StyleCollection.hpp"
#include "TimeController.hpp"

#include "ColorInputNode.hpp"
#include "GradientInputNode.hpp"
#include "MouseInputNode.hpp"
#include "NumberInputNode.hpp"
#include "SceneInputNode.hpp"
#include "SliderInputNode.hpp"
#include "TextInputNode.hpp"
#include "TimeInputNode.hpp"
#include "Vec2InputNode.hpp"
#include "WindowInputNode.hpp"

#include "ATypeNode.hpp"
#include "AdditionNode.hpp"
#include "BlendNode.hpp"
#include "CombineColorNode.hpp"
#include "CombineVec2Node.hpp"
#include "CosNode.hpp"
#include "DimensionNode.hpp"
#include "DivisionNode.hpp"
#include "FillNode.hpp"
#include "ImageDisplayNode.hpp"
#include "MultiplicationNode.hpp"
#include "SinNode.hpp"
#include "SplitColorNode.hpp"
#include "SplitVec2Node.hpp"
#include "StackNode.hpp"
#include "SubtractionNode.hpp"
#include "SurfaceDisplayNode.hpp"
#include "SurfaceLoader.hpp"
#include "TextTyperNode.hpp"
#include "UkrugNode.hpp"
#include "VideoDisplayNode.hpp"

#include "FileManager.hpp"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QtQml>
#include <QtWidgets/QApplication>
#include <gst/gst.h>
#include <memory>
#include <qvariant.h>

// Must be run after QApplication setup
static void setupGstreamer() {
  gst_init(nullptr, nullptr);

  // Register qml6 gstreamer plugin
  GstElement *sink = gst_element_factory_make("qml6glsink", NULL);
  gst_object_unref(sink);
}

static std::shared_ptr<NodeDelegateModelRegistry> createRegistery(QQmlEngine &engine) {
  auto reg = std::make_shared<NodeDelegateModelRegistry>(&engine);

  // Input
  reg->registerModel<ColorInputNode>("Input");
  reg->registerModel<GradientInputNode>("Input");
  reg->registerModel<MouseInputNode>("Input");
  reg->registerModel<NumberInputNode>("Input");
  reg->registerModel<SceneInputNode>("Input");
  reg->registerModel<TextInputNode>("Input");
  reg->registerModel<WindowInputNode>("Input");
  reg->registerModel<TimeInputNode>("Input");
  reg->registerModel<Vec2InputNode>("Input");
  reg->registerModel<SliderInputNode>("Input");

  reg->registerModel<AdditionNode>("Process");
  reg->registerModel<DivisionNode>("Process");
  reg->registerModel<MultiplicationNode>("Process");
  reg->registerModel<SubtractionNode>("Process");
  reg->registerModel<SinNode>("Process");
  reg->registerModel<CosNode>("Process");
  reg->registerModel<CombineColorNode>("Process");
  reg->registerModel<SplitColorNode>("Process");
  reg->registerModel<CombineVec2Node>("Process");
  reg->registerModel<SplitVec2Node>("Process");
  reg->registerModel<TextTyperNode>("Process");

  reg->registerModel<DimensionNode>("Display");
  reg->registerModel<SurfaceDisplayNode>("Display");
  reg->registerModel<UkrugNode>("Display");
  reg->registerModel<ATypeNode>("Display");
  reg->registerModel<BlendNode>("Process");
  reg->registerModel<VideoDisplayNode>("Display");
  reg->registerModel<FillNode>("Display");
  reg->registerModel<StackNode>("Display");
  reg->registerModel<ImageDisplayNode>("Display");
  return reg;
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  setupGstreamer();

  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  engine.rootContext()->setContextProperty("app", &app);

  std::shared_ptr<DataFlowGraphModel> graph =
      std::make_shared<DataFlowGraphModel>(createRegistery(engine), &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  DataFlowModelInterface::init(graph.get());

  StyleCollection::followApplicationPalette(true);
  TimeController::init();

  FileManager fileManager(graph);

  engine.setInitialProperties({{"fileManager", QVariant::fromValue(&fileManager)}});
  engine.loadFromModule("CutieDesigner", "Main");

  QObject &window = *engine.rootObjects().first();
  TimeController::linkCutieWindow(CutieWindow::getCutieWindow(&engine));

  auto source = graph->addNode(SurfaceDisplayNode(&engine).name());
  graph->setNodeData(source, NodeRole::Position, QPointF(750, 225));
  graph->setNodeData(source, NodeRole::Flags, NodeFlags({NodeFlag::Locked}).toInt());
  auto display = graph->delegateModel<SurfaceDisplayNode>(source);

  auto loader = window.property("objectLoader").value<SurfaceLoader *>();
  QObject::connect(display, SIGNAL(contentChanged(SurfaceData *)), loader,
                   SLOT(setSurfaceData(SurfaceData *)));

  int status = app.exec();
  gst_deinit();

  // Delete first surface loader to unload the visual tree
  // before the node tree to prevent missing properties
  delete loader;
  return status;
}