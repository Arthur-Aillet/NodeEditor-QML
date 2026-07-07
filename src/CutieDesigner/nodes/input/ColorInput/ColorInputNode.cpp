#include "ColorInputNode.hpp"
#include "ColorData.hpp"
#include <memory>

ColorInputNode::ColorInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(std::make_shared<ColorData>(_color)) {}

QQmlComponent ColorInputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "ColorInputControl");
}

QVariantMap ColorInputNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
}

QJsonObject ColorInputNode::save() const {
  QJsonObject json;

  json["color"] = QJsonValue::fromVariant(_color);
  return json;
}

void ColorInputNode::load(QJsonObject const &json) {
  _color = json["color"].toVariant().value<QColor>();
}

unsigned int ColorInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType ColorInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return ColorData().type();
}

std::shared_ptr<NodeData> ColorInputNode::outData(PortIndex _portIndex) { return _content; }