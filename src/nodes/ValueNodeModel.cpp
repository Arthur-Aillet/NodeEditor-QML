#include "ValueNodeModel.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qqmlcomponent.h>
#include <qvalidator.h>

ValueNodeModel::ValueNodeModel(QQmlEngine *engine)
    : NodeDelegateModel(engine), _number(std::make_shared<DecimalData>(0.0)) {}

QJsonObject ValueNodeModel::save() const {
  QJsonObject modelJson = NodeDelegateModel::save();

  modelJson["number"] = QString::number(_number->number());

  return modelJson;
}

void ValueNodeModel::load(QJsonObject const &p) {
  QJsonValue v = p["number"];

  if (!v.isUndefined()) {
    QString strNum = v.toString();

    bool ok = false;
    double d = strNum.toDouble(&ok);
    if (ok) {
      _number = std::make_shared<DecimalData>(d);

      // if (_lineEdit)
      //   _lineEdit->setText(strNum);
    }
  }
}

unsigned int ValueNodeModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent ValueNodeModel::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesignerModule", "PortLabel");
}

void ValueNodeModel::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _portLabel = loaded;
  _portLabel->setProperty("placeholderText", "Value");
  if (_number != nullptr) {
    _portLabel->setProperty("text", _number->numberAsText());
  }
  _portLabel->connect(_portLabel.get(), SIGNAL(textEdited()), this, SLOT(onTextEdited()));
}

void ValueNodeModel::onTextEdited() {
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

const NodeDataType &ValueNodeModel::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> ValueNodeModel::outData(PortIndex) { return _number; }

void ValueNodeModel::setNumber(double n) {
  _number = std::make_shared<DecimalData>(n);

  Q_EMIT dataUpdated(0);

  // if (_lineEdit)
  //   _lineEdit->setText(QString::number(_number->number()));
}