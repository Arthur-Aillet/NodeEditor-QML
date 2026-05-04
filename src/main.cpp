#include "DataFlowModelInterface.hpp"
#include "NodeModel.hpp"
#include "QtNodes/internal/DataFlowGraphModel.hpp"
#include "QtNodes/internal/DataFlowGraphicsScene.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include "QtNodes/internal/QmlWrapper.hpp"
#include "ValueNodeModel.hpp"
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>

#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include <memory>
#include <qboxlayout.h>
#include <qqml.h>
#include <qqmlapplicationengine.h>
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
  qputenv("QT_QUICK_BACKEND", "software");
  QApplication app(argc, argv);

  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionNode>("Process");

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

  return app.exec();
}