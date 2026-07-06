#include "Dimension/DimensionNode.hpp"
#include "DecimalData.hpp"
#include "DimensionNode.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

DimensionNode::DimensionNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "Dimension");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
}

unsigned int DimensionNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 5;
  default:
    return 1;
  }
}

NodeDataType DimensionNode::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    if (portIndex == 0)
      return SurfaceData().type();
    return DecimalData().type();
  default:
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> DimensionNode::outData(PortIndex _portIndex) {
  if (_surface.expired())
    return nullptr;
  return _content;
}

void DimensionNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    switch (portIndex) {
    case 0:
      _surface.reset();
      emit dataInvalidated(0);
      emit surfaceChanged();
      return;
    case 1:
      _inX.reset();
      emit inXChanged();
      return;
    case 2:
      _inY.reset();
      emit inYChanged();
      return;
    case 3:
      _inWidth.reset();
      emit inWidthChanged();
      return;
    case 4:
      _inHeight.reset();
      emit inHeightChanged();
    }
  } else {
    switch (portIndex) {
    case 0:
      _surface = std::dynamic_pointer_cast<SurfaceData>(data);
      emit dataUpdated(0);
      emit surfaceChanged();
      return;
    case 1:
      _inX = data;
      emit inXChanged();
      return;
    case 2:
      _inY = data;
      emit inYChanged();
      return;
    case 3:
      _inWidth = data;
      emit inWidthChanged();
      return;
    case 4:
      _inHeight = data;
      emit inHeightChanged();
    }
  }
}

QString DimensionNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return QString("in");
    case 1:
      return QString("x");
    case 2:
      return QString("y");
    case 3:
      return QString("width");
    case 4:
      return QString("height");
    }
  default:
    return QString("out");
  }
}