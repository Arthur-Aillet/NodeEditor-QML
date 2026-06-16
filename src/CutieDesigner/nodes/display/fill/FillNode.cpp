#include "FillNode.hpp"
#include "GradientData.hpp"
#include "SurfaceData.hpp"

FillNode::FillNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "FillSurface");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
  _defaultGradient = QLinearGradient();
  _defaultGradient.setColorAt(0, "red");
}

unsigned int FillNode::nPorts(PortType portType) const { return 1; }

const NodeDataType &FillNode::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return GradientData().type();
  default:
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> FillNode::outData(PortIndex _portIndex) { return _content; }

void FillNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {

  if (!data) {
    _gradient.reset();
    emit gradientChanged();
    emit dataInvalidated(0);
  } else {
    _gradient = data;
    emit gradientChanged();
    emit dataUpdated(0);
  }
}

QString FillNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return "gradient";
  default:
    return "out";
  }
}