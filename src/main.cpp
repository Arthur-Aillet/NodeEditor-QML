#include "DataFlowModelInterface.hpp"
#include "QtNodes/internal/DataFlowGraphModel.hpp"
#include "QtNodes/internal/DataFlowGraphicsScene.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include "QtNodes/internal/QmlWrapper.hpp"
#include "nodes/AdditionModel.hpp"
#include "nodes/DivisionModel.hpp"
#include "nodes/MultiplicationModel.hpp"
#include "nodes/NumberDisplayDataModel.hpp"
#include "nodes/SubtractionModel.hpp"
#include "nodes/ValueNodeModel.hpp"
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <memory>
#include <qboxlayout.h>
#include <qobjectdefs.h>
#include <qqml.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qwidget.h>

using QtNodes::NodeRole;

static QtNodes::DataFlowGraphicsScene *startOriginalNodeEditor(QtNodes::DataFlowGraphModel &model,
                                                               QWidget &mainWidget) {
  QVBoxLayout *l = new QVBoxLayout(&mainWidget);

  auto scene = new DataFlowGraphicsScene(model, &mainWidget);

  auto view = new GraphicsView(scene);
  l->addWidget(view);
  l->setContentsMargins(0, 0, 0, 0);
  l->setSpacing(0);

  QObject::connect(scene, &DataFlowGraphicsScene::sceneLoaded, view, &GraphicsView::centerScene);

  QObject::connect(scene, &DataFlowGraphicsScene::modified, &mainWidget,
                   [&mainWidget]() { mainWidget.setWindowModified(true); });
  mainWidget.setWindowTitle("[*]CutieOriginal");
  mainWidget.resize(800, 600);
  // Center window.
  mainWidget.move(QApplication::primaryScreen()->availableGeometry().center() -
                  mainWidget.rect().center());
  mainWidget.showNormal();
  return scene;
}

int main(int argc, char *argv[]) {
  // qputenv("QT_QUICK_BACKEND", "software");
  QApplication app(argc, argv);

  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionModel>("Process");
  ret->registerModel<DivisionModel>("Process");
  ret->registerModel<MultiplicationModel>("Process");
  ret->registerModel<SubtractionModel>("Process");
  ret->registerModel<NumberDisplayDataModel>("Display");

  auto model = QtNodes::DataFlowGraphModel(ret);

  QWidget mainWidget;
  auto scene = startOriginalNodeEditor(model, mainWidget);

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

  QObject *item = engine.rootObjects().first();

  auto source = model.addNode(NumberDisplayDataModel().name());
  model.setNodeData(source, NodeRole::Position, QPointF(400, 0));
  model.setNodeData(source, NodeRole::Type, NumberDisplayDataModel().name());
  auto display = model.delegateModel<NumberDisplayDataModel>(source);
  QObject::connect(display, SIGNAL(valueUpdated(double)), item, SIGNAL(newDisplayValue(double)));

  return app.exec();
}