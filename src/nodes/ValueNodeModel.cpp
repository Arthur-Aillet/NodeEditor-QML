#include "ValueNodeModel.hpp"
#include "DecimalData.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qvalidator.h>

ValueNodeModel::ValueNodeModel()
    : _lineEdit{nullptr}, _number(std::make_shared<DecimalData>(0.0)) {}

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

      if (_lineEdit)
        _lineEdit->setText(strNum);
    }
  }
}

unsigned int ValueNodeModel::nPorts(PortType portType) const {
  unsigned int result = 1;

  switch (portType) {
  case PortType::In:
    result = 0;
    break;

  case PortType::Out:
    result = 1;

  default:
    break;
  }

  return result;
}

void ValueNodeModel::onTextEdited(QString const &str) {
  bool ok = false;

  double number = str.toDouble(&ok);

  if (ok) {
    _number = std::make_shared<DecimalData>(number);

    Q_EMIT dataUpdated(0);

  } else {
    Q_EMIT dataInvalidated(0);
  }
}

NodeDataType ValueNodeModel::dataType(PortType, PortIndex) const { return DecimalData().type(); }

std::shared_ptr<NodeData> ValueNodeModel::outData(PortIndex) { return _number; }

QWidget *ValueNodeModel::embeddedWidget() {
  if (!_lineEdit) {
    _lineEdit = new QLineEdit();

    _lineEdit->setValidator(new QDoubleValidator());
    _lineEdit->setMaximumSize(_lineEdit->sizeHint());

    connect(_lineEdit, &QLineEdit::textChanged, this, &ValueNodeModel::onTextEdited);

    _lineEdit->setText(QString::number(_number->number()));
  }

  return _lineEdit;
}

void ValueNodeModel::setNumber(double n) {
  _number = std::make_shared<DecimalData>(n);

  Q_EMIT dataUpdated(0);

  if (_lineEdit)
    _lineEdit->setText(QString::number(_number->number()));
}