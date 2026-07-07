#include "CutieWindow.hpp"
#include "DataFlowModelInterface.hpp"
#include "StyleCollection.hpp"
#include "TimeController.hpp"

// #include "ATypeNode.hpp"
// #include "AdditionNode.hpp"
// #include "BlendNode.hpp"
// #include "ColorInputNode.hpp"
// #include "CombineColorNode.hpp"
// #include "CombineVec2.hpp"
// #include "CosNode.hpp"
// #include "Definitions.hpp"
// #include "DimensionNode.hpp"
// #include "DivisionNode.hpp"
// #include "FillNode.hpp"
// #include "GradientInputNode.hpp"
// #include "ImageDisplayNode.hpp"
// #include "MouseInputNode.hpp"
// #include "MultiplicationNode.hpp"
// #include "NumberInputNode.hpp"
// #include "SceneInputNode.hpp"
// #include "SinNode.hpp"
// #include "SliderInputNode.hpp"
// #include "SplitColorNode.hpp"
// #include "SplitVec2.hpp"
// #include "StackNode.hpp"
// #include "SubtractionNode.hpp"
// #include "SurfaceDisplayNode.hpp"
// #include "SurfaceLoader.hpp"
// #include "TextInputNode.hpp"
// #include "TextTyperNode.hpp"
// #include "TimeInputNode.hpp"
// #include "UkrugNode.hpp"
// #include "Vec2InputNode.hpp"
// #include "VideoDisplayNode.hpp"
// #include "WindowInputNode.hpp"

#include "TimeInputNode.hpp"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QtQml>
#include <QtWidgets/QApplication>
#include <gst/gst.h>
#include <memory>

// Must be run after QApplication setup
static void setupGstreamer() {
  gst_init(nullptr, nullptr);

  // Register qml6 gstreamer plugin
  GstElement *sink = gst_element_factory_make("qml6glsink", NULL);
  gst_object_unref(sink);
}

static std::shared_ptr<NodeDelegateModelRegistry> createRegistery(QQmlEngine &engine) {
  auto reg = std::make_shared<NodeDelegateModelRegistry>(&engine);
  reg->registerModel<TimeInputNode>("Input");

  // reg->registerModel<ColorInputNode>("Input");
  // reg->registerModel<GradientInputNode>("Input");
  // reg->registerModel<NumberInputNode>("Input");
  // reg->registerModel<Vec2InputNode>("Input");
  // reg->registerModel<MouseInputNode>("Input");
  // reg->registerModel<WindowInputNode>("Input");
  // reg->registerModel<SceneInputNode>("Input");
  // reg->registerModel<TextInputNode>("Input");
  // reg->registerModel<TimeInputNode>("Input");
  // reg->registerModel<SliderInputNode>("Input");
  // reg->registerModel<AdditionNode>("Process");
  // reg->registerModel<DivisionNode>("Process");
  // reg->registerModel<MultiplicationNode>("Process");
  // reg->registerModel<SubtractionNode>("Process");
  // reg->registerModel<SinNode>("Process");
  // reg->registerModel<CosNode>("Process");
  // reg->registerModel<CombineColorNode>("Process");
  // reg->registerModel<SplitColorNode>("Process");
  // reg->registerModel<CombineVec2>("Process");
  // reg->registerModel<SplitVec2>("Process");
  // reg->registerModel<DimensionNode>("Display");
  // reg->registerModel<SurfaceDisplayNode>("Display");
  // reg->registerModel<UkrugNode>("Display");
  // reg->registerModel<ATypeNode>("Display");
  // reg->registerModel<TextTyperNode>("Process");
  // reg->registerModel<BlendNode>("Process");
  // reg->registerModel<VideoDisplayNode>("Display");
  // reg->registerModel<FillNode>("Display");
  // reg->registerModel<StackNode>("Display");
  // reg->registerModel<ImageDisplayNode>("Display");
  return reg;
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  setupGstreamer();

  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  engine.rootContext()->setContextProperty("app", &app);

  auto graph = DataFlowGraphModel(createRegistery(engine), &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  DataFlowModelInterface::init(graph);

  StyleCollection::followApplicationPalette(true);
  TimeController::init();
  engine.loadFromModule("CutieDesigner", "Main");
  TimeController::linkCutieWindow(CutieWindow::getCutieWindow(&engine));

  QObject &item = *engine.rootObjects().first();

  // auto source = model.addNode(SurfaceDisplayNode(&engine).name());
  // model.setNodeData(source, NodeRole::Position, QPointF(750, 225));
  // model.setNodeData(source, NodeRole::Type, SurfaceDisplayNode(&engine).name());
  // model.setNodeData(source, NodeRole::Flags, NodeFlag::Locked);
  // auto display = model.delegateModel<SurfaceDisplayNode>(source);

  // auto loader = item.property("objectLoader").value<SurfaceLoader *>();
  // QObject::connect(display, SIGNAL(contentChanged(SurfaceData *)), loader,
  //                  SLOT(setSurfaceData(SurfaceData *)));

  int status = app.exec();
  gst_deinit();

  // Delete first surface loader to unload the visual tree
  // before the node tree to prevent missing properties
  // delete loader;
  return status;
}