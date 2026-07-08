#include "BlendNode.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

BlendNode::BlendNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "Blend");
  _content = std::make_shared<SurfaceData>(std::move(comp),
                                           QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int BlendNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  default:
    return 1;
  }
}

NodeDataType BlendNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> BlendNode::outData(PortIndex _portIndex) {
  if (_a.expired() && _b.expired())
    return nullptr;
  return _content;
}

void BlendNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    if (portIndex == 0) {
      _a.reset();
      emit aChanged();
    } else {
      _b.reset();
      emit bChanged();
    }
    emit dataInvalidated(0);
  } else {
    if (portIndex == 0) {
      _a = std::dynamic_pointer_cast<SurfaceData>(data);
      emit aChanged();
    } else {
      _b = std::dynamic_pointer_cast<SurfaceData>(data);
      emit bChanged();
    }
    emit dataUpdated(0);
  }
}

QString BlendNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return QString("a");
    default:
      return QString("b");
    }
  default:
    return QString("out");
  }
}