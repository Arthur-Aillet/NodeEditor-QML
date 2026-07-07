#include "NumberInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qqmlcomponent.h>

NumberInputNode::NumberInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _numberData(std::make_shared<DecimalData>(_number)) {}

QJsonObject NumberInputNode::save() const { return QJsonObject({{"number", _number}}); }

void NumberInputNode::load(QJsonObject const &json) {
  QJsonValue value = json["number"];

  if (!value.isUndefined()) {
    _number = value.toDouble();
  }
}

unsigned int NumberInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent NumberInputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "NumberInputControl");
}

void NumberInputNode::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _portLabel = loaded;
  _portLabel->setProperty("placeholderText", "Value");
  if (_numberData != nullptr) {
    _portLabel->setProperty("text", _numberData->repr<QString>());
  }
  _portLabel->connect(_portLabel.get(), SIGNAL(textEdited()), this, SLOT(onTextEdited()));
}

void NumberInputNode::onTextEdited() {
  auto str = _portLabel->property("text");

  if (str == "") {
    _number = 0;
    Q_EMIT dataUpdated(0);
    return;
  }

  bool ok = false;
  double number = str.toDouble(&ok);

  if (ok) {
    _number = number;
    Q_EMIT dataUpdated(0);
  } else {
    Q_EMIT dataInvalidated(0);
  }
}

NodeDataType NumberInputNode::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> NumberInputNode::outData(PortIndex) { return _numberData; }