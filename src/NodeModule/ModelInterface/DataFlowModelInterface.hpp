#pragma once

#include "ModelInterface.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <memory>
#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qforeach.h>
#include <qjsengine.h>
#include <qlist.h>
#include <qloggingcategory.h>
#include <qnamespace.h>
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
  Q_PROPERTY(QStandardItemModel *nodeMapModel MEMBER m_nodeMapModel NOTIFY nodeMapModelChanged)

  Q_INVOKABLE QStandardItemModel *filterNodeMapModel(QString filter) {
    m_nodeMapModel->clear();
    QStandardItem *root = m_nodeMapModel->invisibleRootItem();

    auto categoryList = QStringList(registry->categories().begin(), registry->categories().end());
    categoryList.sort(Qt::CaseInsensitive);
    for (const auto &category : categoryList) {
      const auto &map = registry->registeredModelsCategoryAssociation();
      QStringList row;

      for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->second == category && it->first.contains(filter, Qt::CaseInsensitive)) {
          row.push_back(it->first);
        }
      }
      row.sort(Qt::CaseInsensitive);

      if (!row.empty()) {
        QStandardItem *categoryItem = new QStandardItem(category);

        for (auto val : row) {
          categoryItem->appendRow(new QStandardItem(val));
        }
        root->appendRow(categoryItem);
      }
    }

    emit nodeMapModelChanged();
    return m_nodeMapModel;
  }

  QStandardItemModel *m_nodeMapModel = nullptr;

  RegisteryAccess() {}
  RegisteryAccess(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _reg)
      : registry(_reg), m_nodeMapModel(new QStandardItemModel()) {
    filterNodeMapModel("");
  }

  bool operator==(const RegisteryAccess &other) { return registry == other.registry; }

  signals:
  void nodeMapModelChanged();
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
