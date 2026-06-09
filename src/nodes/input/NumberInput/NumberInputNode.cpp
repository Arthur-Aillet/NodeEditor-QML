#include "NumberInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qqmlcomponent.h>
#include <qvalidator.h>

NumberInputNode::NumberInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _number(std::make_shared<DecimalData>(0.0)) {}

QJsonObject NumberInputNode::save() const {
  QJsonObject modelJson = NodeDelegateModel::save();

  modelJson["number"] = QString::number(_number->number());

  return modelJson;
}

void NumberInputNode::load(QJsonObject const &p) {
  QJsonValue v = p["number"];

  if (!v.isUndefined()) {
    QString strNum = v.toString();

    bool ok = false;
    double d = strNum.toDouble(&ok);
    if (ok) {
      _number = std::make_shared<DecimalData>(d);
    }
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
  return QQmlComponent(engine, "CutieDesignerModule", "NumberInputControl");
}

void NumberInputNode::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _portLabel = loaded;
  _portLabel->setProperty("placeholderText", "Value");
  if (_number != nullptr) {
    _portLabel->setProperty("text", _number->numberAsText()->text);
  }
  _portLabel->connect(_portLabel.get(), SIGNAL(textEdited()), this, SLOT(onTextEdited()));
}

void NumberInputNode::onTextEdited() {
  auto str = _portLabel->property("text");
  bool ok = false;

  double number = str.toDouble(&ok);

  if (ok) {
    _number = std::make_shared<DecimalData>(number);

    Q_EMIT dataUpdated(0);

  } else {
    Q_EMIT dataInvalidated(0);
  }
}

const NodeDataType &NumberInputNode::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> NumberInputNode::outData(PortIndex) { return _number; }

void NumberInputNode::setNumber(double n) {
  _number = std::make_shared<DecimalData>(n);

  emit dataUpdated(0);
}