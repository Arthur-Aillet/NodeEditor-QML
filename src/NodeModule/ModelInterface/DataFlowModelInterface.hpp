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

class RegisteryAccess {
  Q_GADGET
  QML_VALUE_TYPE(registeryAccess)

  std::shared_ptr<QtNodes::NodeDelegateModelRegistry> reg;

  public:
  Q_INVOKABLE QtNodes::NodeDelegateModelRegistry::CategoriesSet categories() {
    return reg->categories();
  }

  RegisteryAccess(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _reg = nullptr)
      : reg(_reg) {};
};

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::DataFlowGraphModel &graphModel;

  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static DataFlowModelInterface *init(QtNodes::DataFlowGraphModel &_graphModel);

  Q_INVOKABLE RegisteryAccess dataModelRegistry() {
    return RegisteryAccess(graphModel.dataModelRegistry());
  };

  protected:
  DataFlowModelInterface(QtNodes::DataFlowGraphModel &_graphModel);
};
