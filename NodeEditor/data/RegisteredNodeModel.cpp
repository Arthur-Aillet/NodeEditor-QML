#include "RegisteredNodeModel.hpp"
#include <qvariant.h>

int RegisteredNodeModel::rowCount(const QModelIndex &parent) const { return _nodes.size(); }

QVariant RegisteredNodeModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case static_cast<int>(RegisteredNodeModel::NodeModelRoles::Name):
    return _nodes[index.row()].first.name;
  case static_cast<int>(RegisteredNodeModel::NodeModelRoles::Category):
    return _nodes[index.row()].second;
  case static_cast<int>(RegisteredNodeModel::NodeModelRoles::PortsInfo):
    return QVariant::fromValue(_nodes[index.row()].first.ports);
  default:
    return QVariant::fromMetaType(QMetaType(QMetaType::UnknownType));
  }
}

void RegisteredNodeModel::registerNode(NodeDelegateModel::ModelInfos nodeModelInfo,
                                       QString category) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  _nodes.push_back(std::pair(nodeModelInfo, category));
  endInsertRows();
}
