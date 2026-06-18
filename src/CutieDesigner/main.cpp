#include "ATypeNode.hpp"
#include "AdditionNode.hpp"
#include "BlendNode.hpp"
#include "ColorInputNode.hpp"
#include "CombineRGBA.hpp"
#include "CombineVec2.hpp"
#include "CosNode.hpp"
#include "DataFlowModelInterface.hpp"
#include "Definitions.hpp"
#include "DimensionNode.hpp"
#include "DivisionNode.hpp"
#include "FillNode.hpp"
#include "GradientInputNode.hpp"
#include "MouseInputNode.hpp"
#include "MultiplicationNode.hpp"
#include "NumberInputNode.hpp"
#include "SceneInputNode.hpp"
#include "SinNode.hpp"
#include "SplitRGBA.hpp"
#include "SplitVec2.hpp"
#include "StackNode.hpp"
#include "SubtractionNode.hpp"
#include "SurfaceDisplayNode.hpp"
#include "SurfaceLoader.hpp"
#include "TextTyperNode.hpp"
#include "UkrugNode.hpp"
#include "Vec2InputNode.hpp"
#include "VideoDisplayNode.hpp"
#include "WindowInputNode.hpp"

#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtWidgets/QApplication>
#include <gst/gst.h>
#include <memory>
#include <qdebug.h>
#include <qquickitem.h>

int main(int argc, char *argv[]) {
  gst_init(nullptr, nullptr);

  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  engine.rootContext()->setContextProperty("app", &app);

  auto ret = std::make_shared<NodeDelegateModelRegistry>(&engine);

  ret->registerModel<ColorInputNode>("Input");
  ret->registerModel<GradientInputNode>("Input");
  ret->registerModel<NumberInputNode>("Input");
  ret->registerModel<Vec2InputNode>("Input");
  ret->registerModel<MouseInputNode>("Input");
  ret->registerModel<WindowInputNode>("Input");
  ret->registerModel<SceneInputNode>("Input");
  ret->registerModel<AdditionNode>("Process");
  ret->registerModel<DivisionNode>("Process");
  ret->registerModel<MultiplicationNode>("Process");
  ret->registerModel<SubtractionNode>("Process");
  ret->registerModel<SinNode>("Process");
  ret->registerModel<CosNode>("Process");
  ret->registerModel<CombineRGBA>("Process");
  ret->registerModel<SplitRGBA>("Process");
  ret->registerModel<CombineVec2>("Process");
  ret->registerModel<SplitVec2>("Process");
  ret->registerModel<DimensionNode>("Display");
  ret->registerModel<SurfaceDisplayNode>("Display");
  ret->registerModel<UkrugNode>("Display");
  ret->registerModel<ATypeNode>("Display");
  ret->registerModel<TextTyperNode>("Process");
  ret->registerModel<BlendNode>("Process");
  ret->registerModel<VideoDisplayNode>("Display");
  ret->registerModel<FillNode>("Display");
  ret->registerModel<StackNode>("Display");

  GstElement *sink = gst_element_factory_make("qml6glsink", NULL);
  gst_object_unref(sink);

  auto model = DataFlowGraphModel(ret, &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesigner", "Main");
  QObject &item = *engine.rootObjects().first();

  auto source = model.addNode(SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Position, QPointF(750, 225));
  model.setNodeData(source, NodeRole::Type, SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Flags, NodeFlag::Locked);
  auto display = model.delegateModel<SurfaceDisplayNode>(source);

  auto loader = item.property("objectLoader").value<SurfaceLoader *>();
  QObject::connect(display, SIGNAL(contentChanged(SurfaceData *)), loader,
                   SLOT(setSurfaceData(SurfaceData *)));

  int status = app.exec();
  gst_deinit();

  // Delete first surface loader to unload the visual tree
  // before the node tree to prevent missing properties
  delete loader;
  return status;
}