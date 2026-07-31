#include "Vec2InputNode.hpp"
#include "NodeDelegateModel.hpp"
#include "Vec2Data.hpp"
#include <qvariant.h>

Vec2InputNode::Vec2InputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _vecData(std::make_shared<Vec2Data>(_vec)) {}

QJsonObject Vec2InputNode::save() const { return QJsonObject({{"x", _x}, {"y", _y}}); }

void Vec2InputNode::load(QJsonObject const &json) {
  QJsonValue valueX = json["x"];

  if (!valueX.isUndefined()) {
    _vec.setX(valueX.toDouble());
    _x = valueX.toDouble();
  }

  QJsonValue valueY = json["y"];
  if (!valueY.isUndefined()) {
    _vec.setY(valueY.toDouble());
    _y = valueY.toDouble();
  }
}

unsigned int Vec2InputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent Vec2InputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "Vec2InputControl");
}

QVariantMap Vec2InputNode::componentInitialProperties() {
  return QVariantMap{{"node", QVariant::fromValue(this)}};
}

NodeDataType Vec2InputNode::dataType(PortType, PortIndex id) const { return Vec2Data().type(); }

std::shared_ptr<NodeData> Vec2InputNode::outData(PortIndex) { return _vecData; }

void Vec2InputNode::setX(double x) {
  if (x == _x)
    return;
  _x = x;
  _vec.setX(x);
  emit xChanged();
  emit dataUpdated(0);
}

void Vec2InputNode::setY(double y) {
  if (y == _y)
    return;
  _y = y;
  _vec.setY(y);
  emit yChanged();
  emit dataUpdated(0);
}