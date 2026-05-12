#include "DataFlowModelInterface.hpp"

#include "AdditionModel.hpp"
#include "Definitions.hpp"
#include "DivisionModel.hpp"
#include "MultiplicationModel.hpp"
#include "NumberDisplayDataModel.hpp"
#include "SubtractionModel.hpp"
#include "ValueNodeModel.hpp"

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <memory>
#include <qboxlayout.h>
#include <qlogging.h>
#include <qobjectdefs.h>
#include <qqml.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qwidget.h>

// static QtNodes::DataFlowGraphicsScene *startOriginalNodeEditor(QtNodes::DataFlowGraphModel
// &model,
//                                                                QWidget &mainWidget) {
//   QVBoxLayout *l = new QVBoxLayout(&mainWidget);

//   auto scene = new QtNodes::DataFlowGraphicsScene(model, &mainWidget);

//   auto view = new QtNodes::GraphicsView(scene);
//   l->addWidget(view);
//   l->setContentsMargins(0, 0, 0, 0);
//   l->setSpacing(0);

//   QObject::connect(scene, &QtNodes::DataFlowGraphicsScene::sceneLoaded, view,
//                    &QtNodes::GraphicsView::centerScene);

//   QObject::connect(scene, &QtNodes::DataFlowGraphicsScene::modified, &mainWidget,
//                    [&mainWidget]() { mainWidget.setWindowModified(true); });
//   mainWidget.setWindowTitle("[*]CutieOriginal");
//   mainWidget.resize(800, 600);
//   // Center window.
//   mainWidget.move(QApplication::primaryScreen()->availableGeometry().center() -
//                   mainWidget.rect().center());
//   mainWidget.showNormal();
//   return scene;
// }

int main(int argc, char *argv[]) {
  // qputenv("QT_QUICK_BACKEND", "software");
  QApplication app(argc, argv);

  auto ret = std::make_shared<NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionModel>("Process");
  ret->registerModel<DivisionModel>("Process");
  ret->registerModel<MultiplicationModel>("Process");
  ret->registerModel<SubtractionModel>("Process");
  ret->registerModel<NumberDisplayDataModel>("Display");

  auto model = DataFlowGraphModel(ret);

  // QWidget mainWidget;
  // auto scene = startOriginalNodeEditor(model, mainWidget);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  DataFlowModelInterface::init(model);

  engine.loadFromModule("CutieDesignerModule", "Main");
  {
    auto source = model.addNode(ValueNodeModel().name());
    model.setNodeData(source, NodeRole::Position, QPointF(0, 0));
    model.setNodeData(source, NodeRole::Type, ValueNodeModel().name());
  }

  engine.rootContext()->setContextProperty("InvalidNodeId", InvalidNodeId);
  engine.rootContext()->setContextProperty("InvalidPortIndex", InvalidPortIndex);
  engine.rootContext()->setContextProperty("InvalidGroupId", InvalidGroupId);

  QObject *item = engine.rootObjects().first();

  auto source = model.addNode(NumberDisplayDataModel().name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 0));
  model.setNodeData(source, NodeRole::Type, NumberDisplayDataModel().name());
  auto display = model.delegateModel<NumberDisplayDataModel>(source);
  QObject::connect(display, SIGNAL(valueUpdated(double)), item, SIGNAL(newDisplayValue(double)));

  return app.exec();
}