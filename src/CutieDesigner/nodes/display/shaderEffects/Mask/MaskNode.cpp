#include "MaskNode.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

MaskNode::MaskNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "Mask");
  _content = std::make_shared<SurfaceData>(std::move(comp),
                                           QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int MaskNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  default:
    return 1;
  }
}

NodeDataType MaskNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> MaskNode::outData(PortIndex _portIndex) {
  if (_image.expired() && _mask.expired())
    return nullptr;
  return _content;
}

void MaskNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    if (portIndex == 0) {
      _image.reset();
      emit imageChanged();
    } else {
      _mask.reset();
      emit maskChanged();
    }
    emit dataInvalidated(0);
  } else {
    if (portIndex == 0) {
      _image = std::dynamic_pointer_cast<SurfaceData>(data);
      emit imageChanged();
    } else {
      _mask = std::dynamic_pointer_cast<SurfaceData>(data);
      emit maskChanged();
    }
    emit dataUpdated(0);
  }
}

QString MaskNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return QString("image");
    default:
      return QString("mask");
    }
  default:
    return QString("out");
  }
}