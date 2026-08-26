#pragma once

#include "NodeDelegateModel.hpp"

#include <QObject>
#include <QtQmlIntegration>
#include <vector>

namespace NodeEditor {
class RegisteredNodeModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Created by the DataFlow registry")

  public:
  RegisteredNodeModel() = default;

  enum class NodeModelRoles {
    Name = Qt::DisplayRole,
    Category = Qt::UserRole + 1,
    PortsInfo = Qt::UserRole + 2,
  };

  void registerNode(NodeDelegateModel::ModelInfos nodeModelInfo, QString category);

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(NodeModelRoles::Name), "name"},
            {static_cast<int>(NodeModelRoles::Category), "category"},
            {static_cast<int>(NodeModelRoles::PortsInfo), "portsInfo"}};
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  protected:
  std::vector<std::pair<NodeDelegateModel::ModelInfos, QString>> _nodes;
};
} // namespace NodeEditor
