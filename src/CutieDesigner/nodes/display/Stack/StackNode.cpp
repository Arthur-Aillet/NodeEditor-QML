#include "StackNode.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

int SurfaceList::rowCount(const QModelIndex &parent) const {
  int i = 0;
  for (auto &currentSurface : _surfaces) {
    if (!currentSurface.expired())
      i++;
  }
  return i;
}

QVariant SurfaceList::data(const QModelIndex &index, int role) const {
  int i = 0;
  for (auto &currentSurface : _surfaces) {
    if (!currentSurface.expired()) {
      if (i == index.row())
        return QVariant::fromValue(currentSurface.lock().get());
      i++;
    }
  }
  return QVariant::fromValue(nullptr);
}

void SurfaceList::setPort(std::shared_ptr<SurfaceData> surface, int nodeIndex) {
  int count = 0;

  for (int i = 0; i != nodeIndex + 1; i++) {
    if (!_surfaces[i].expired())
      count++;
  }

  if (surface == nullptr) {
    beginRemoveRows({}, count - 1, count - 1);
    _surfaces[nodeIndex].reset();
    endRemoveRows();
  } else if (_surfaces[nodeIndex].expired()) {
    beginInsertRows({}, count, count);
    _surfaces[nodeIndex] = surface;
    endInsertRows();
  } else {
    _surfaces[nodeIndex] = surface;
    emit dataChanged(QAbstractItemModel::createIndex(count - 1, 0),
                     QAbstractItemModel::createIndex(count - 1, 0));
  }
}

void SurfaceList::addEmptyPort() { _surfaces.push_back({}); }

void SurfaceList::removeLastPort() {
  if (_surfaces.size() <= 1)
    return;

  if (!_surfaces[_surfaces.size() - 1].expired()) {
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    _surfaces.pop_back();
    endRemoveRows();
  } else {
    _surfaces.pop_back();
  }
  return;
}

StackNode::StackNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes", "Stack");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
}

void StackNode::addEmptyPort() {
  emit portsAboutToBeInserted(PortType::In, surfaceList._surfaces.size(),
                              surfaceList._surfaces.size());
  surfaceList.addEmptyPort();
  emit portsInserted(PortType::In);
}

void StackNode::removeLastPort() {
  if (surfaceList._surfaces.size() <= 1)
    return;

  emit portsAboutToBeDeleted(PortType::In, surfaceList._surfaces.size() - 1,
                             surfaceList._surfaces.size() - 1);
  surfaceList.removeLastPort();
  emit portsDeleted(PortType::In);
}

unsigned int StackNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return surfaceList._surfaces.size();
  default:
    return 1;
  }
}

const NodeDataType &StackNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> StackNode::outData(PortIndex _portIndex) { return _content; }

void StackNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    surfaceList.setPort(nullptr, portIndex);
    emit dataUpdated(0);
  } else {
    auto surface = std::dynamic_pointer_cast<SurfaceData>(data);
    surfaceList.setPort(surface, portIndex);
    emit dataUpdated(0);
  }
}

QString StackNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return QString();
  default:
    return QString("out");
  }
}