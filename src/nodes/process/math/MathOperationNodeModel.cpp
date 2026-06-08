#include "MathOperationNodeModel.hpp"
#include "DecimalData.hpp"
#include <qdebug.h>

unsigned int MathOperationNodeModel::nPorts(PortType portType) const {
  if (portType == PortType::In)
    return nbInputs();
  else
    return 1;
}

const NodeDataType &MathOperationNodeModel::dataType(PortType, PortIndex) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> MathOperationNodeModel::outData(PortIndex) {
  return std::static_pointer_cast<NodeData>(_result);
}

void MathOperationNodeModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  auto numberData = std::dynamic_pointer_cast<DecimalData>(data);

  if (!data) {
    emit dataInvalidated(0);
  }

  _inputNumbers[portIndex] = numberData;
  compute();
}
