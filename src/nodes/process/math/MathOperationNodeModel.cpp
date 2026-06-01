#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"
#include <qdebug.h>

unsigned int MathOperationNodeModel::nPorts(PortType portType) const {
  unsigned int result;

  if (portType == PortType::In)
    result = 2;
  else
    result = 1;

  return result;
}

const NodeDataType &MathOperationNodeModel::dataType(PortType, PortIndex) const {
  qDebug() << DecimalData().type().id;
  return DecimalData().type();
}

std::shared_ptr<NodeData> MathOperationNodeModel::outData(PortIndex) {
  return std::static_pointer_cast<NodeData>(_result);
}

void MathOperationNodeModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  auto numberData = std::dynamic_pointer_cast<DecimalData>(data);

  if (!data) {
    Q_EMIT dataInvalidated(0);
  }

  if (portIndex == 0) {
    _number1 = numberData;
  } else {
    _number2 = numberData;
  }

  compute();
}
