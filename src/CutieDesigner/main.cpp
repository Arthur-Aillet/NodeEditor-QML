#include "ATypeNode.hpp"
#include "AdditionNode.hpp"
#include "BlendNode.hpp"
#include "ColorInputNode.hpp"
#include "CombineRGBA.hpp"
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
#include "SinNode.hpp"
#include "SplitRGBA.hpp"
#include "StackNode.hpp"
#include "SubtractionNode.hpp"
#include "SurfaceDisplayNode.hpp"
#include "SurfaceLoader.hpp"
#include "TextTyperNode.hpp"
#include "UkrugNode.hpp"
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

  engine.rootContext()->setContextProperty("app", &app);

  auto ret = std::make_shared<NodeDelegateModelRegistry>(&engine);

  ret->registerModel<ColorInputNode>("Input");
  ret->registerModel<GradientInputNode>("Input");
  ret->registerModel<NumberInputNode>("Input");
  ret->registerModel<MouseInputNode>("Input");
  ret->registerModel<WindowInputNode>("Input");
  ret->registerModel<AdditionNode>("Process");
  ret->registerModel<DivisionNode>("Process");
  ret->registerModel<MultiplicationNode>("Process");
  ret->registerModel<SubtractionNode>("Process");
  ret->registerModel<SinNode>("Process");
  ret->registerModel<CosNode>("Process");
  ret->registerModel<CombineRGBA>("Process");
  ret->registerModel<SplitRGBA>("Process");
  ret->registerModel<DimensionNode>("Display");
  ret->registerModel<SurfaceDisplayNode>("Display");
  ret->registerModel<UkrugNode>("Display");
  ret->registerModel<ATypeNode>("Display");
  ret->registerModel<TextTyperNode>("Process");
  ret->registerModel<BlendNode>("Process");
  ret->registerModel<VideoDisplayNode>("Display");
  ret->registerModel<FillNode>("Display");
  ret->registerModel<StackNode>("Display");

  auto model = DataFlowGraphModel(ret, &engine);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesigner", "Main");
  QObject &item = *engine.rootObjects().first();

  auto source = model.addNode(SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 150));
  model.setNodeData(source, NodeRole::Type, SurfaceDisplayNode(&engine).name());
  model.setNodeData(source, NodeRole::Flags, NodeFlag::Locked);
  auto display = model.delegateModel<SurfaceDisplayNode>(source);

  auto loader = item.property("objectLoader").value<SurfaceLoader *>();
  QObject::connect(display, SIGNAL(contentChanged(SurfaceData *)), loader,
                   SLOT(setSurfaceData(SurfaceData *)));

  auto id1 = model.addNode(ATypeNode(&engine).name());
  model.setNodeData(id1, NodeRole::Position, QPointF(220, 40));
  model.setNodeData(id1, NodeRole::Type, ATypeNode(&engine).name());

  auto id2 = model.addNode(TextTyperNode(&engine).name());
  model.setNodeData(id2, NodeRole::Position, QPointF(0, 0));
  model.setNodeData(id2, NodeRole::Type, TextTyperNode(&engine).name());

  auto ukr = model.addNode(UkrugNode(&engine).name());
  model.setNodeData(ukr, NodeRole::Position, QPointF(100, 150));
  model.setNodeData(ukr, NodeRole::Type, UkrugNode(&engine).name());

  auto id3 = model.addNode(BlendNode(&engine).name());
  model.setNodeData(id3, NodeRole::Position, QPointF(200, 150));
  model.setNodeData(id3, NodeRole::Type, BlendNode(&engine).name());

  int status = app.exec();

  // Delete first surface loader to unload the visual tree
  // before the node tree to prevent missing properties
  delete loader;
  return status;
}