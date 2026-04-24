#pragma once

#include "ModelInterface.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <memory>
#include <qabstractitemmodel.h>
#include <qdebug.h>
#include <qforeach.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qstandarditemmodel.h>
#include <qtmetamacros.h>
#include <qvariant.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class RegisteryAccess {
  Q_GADGET
  QML_VALUE_TYPE(registeryAccess)

  std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = nullptr;

  public:
  Q_PROPERTY(QStandardItemModel *nodeMapModel READ getNodeMapModel)

  QStandardItemModel *getNodeMapModel() {
    QStandardItem *root = model->invisibleRootItem();

    for (const auto &category : registry->categories()) {
      QStandardItem *item = new QStandardItem(category);

      const auto &map = registry->registeredModelsCategoryAssociation();
      for (auto it = map.begin(); it != map.end(); ++it)
        if (it->second == category)
          item->appendRow(new QStandardItem(it->first));
      root->appendRow(item);
    }

    return model;
  }

  QStandardItemModel *model = nullptr;

  RegisteryAccess() {}
  RegisteryAccess(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _reg)
      : registry(_reg), model(new QStandardItemModel()) {}

  bool operator==(const RegisteryAccess &other) { return registry == other.registry; }
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
