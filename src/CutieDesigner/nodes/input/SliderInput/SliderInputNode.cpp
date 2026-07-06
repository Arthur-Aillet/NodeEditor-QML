#include "SliderInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qqmlcomponent.h>

SliderInputNode::SliderInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _valueData(std::make_shared<DecimalData>(_value)) {
  connect(this, &SliderInputNode::valueChanged, [&]() { emit dataUpdated(0); });
}

QJsonObject SliderInputNode::save() const {
  QJsonObject modelJson = NodeDelegateModel::save();

  modelJson["number"] = _valueData->repr<QString>();

  return modelJson;
}

void SliderInputNode::load(QJsonObject const &p) {
  QJsonValue v = p["number"];

  if (!v.isUndefined()) {
    QString strNum = v.toString();

    bool ok = false;
    _value = strNum.toDouble(&ok);
    if (ok) {
      _valueData = std::make_shared<DecimalData>(_value);
    }
  }
}

unsigned int SliderInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent SliderInputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "SliderInputControl");
}

QVariantMap SliderInputNode::componentInitialProperties() {
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  return map;
}

NodeDataType SliderInputNode::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> SliderInputNode::outData(PortIndex) { return _valueData; }