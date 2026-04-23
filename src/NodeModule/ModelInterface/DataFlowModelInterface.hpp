#pragma once

#include "ModelInterface.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <memory>
#include <qdebug.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qstringlistmodel.h>
#include <qtmetamacros.h>
#include <qvariant.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class RegisteryAccess {
  Q_GADGET
  QML_VALUE_TYPE(registeryAccess)

  std::shared_ptr<QtNodes::NodeDelegateModelRegistry> reg = nullptr;

  public:
  Q_PROPERTY(const QSet<QString> &categories READ getCategories)
  Q_PROPERTY(QStringListModel *categoryModel READ getModel)

  const QtNodes::NodeDelegateModelRegistry::CategoriesSet &getCategories() {
    return reg->categories();
  }

  QStringListModel *getModel() {
    qDebug() << model->stringList();
    return model;
  }
  QStringListModel *model = nullptr;

  RegisteryAccess() {}
  RegisteryAccess(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _reg)
      : reg(_reg), model(new QStringListModel(_reg->categories().values())) {}

  bool operator==(const RegisteryAccess &other) { return reg == other.reg; }
};

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::DataFlowGraphModel &graphModel;
  RegisteryAccess registeryAccess;

  Q_PROPERTY(RegisteryAccess registery MEMBER registeryAccess NOTIFY registeryChanged)

  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static DataFlowModelInterface *init(QtNodes::DataFlowGraphModel &_graphModel);

  signals:
  void registeryChanged();

  protected:
  DataFlowModelInterface(QtNodes::DataFlowGraphModel &_graphModel);
};
