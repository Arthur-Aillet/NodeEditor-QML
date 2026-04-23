#pragma once

#include "ModelInterface.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <qdebug.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::DataFlowGraphModel &graphModel;

  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static DataFlowModelInterface *init(QtNodes::DataFlowGraphModel &_graphModel);

  protected:
  DataFlowModelInterface(QtNodes::DataFlowGraphModel &_graphModel);
};
