#include "MathOperationNodeModel.hpp"
#include "DecimalData.hpp"

unsigned int MathOperationNodeModel::nPorts(PortType portType) const {
  if (portType == PortType::In)
    return nbInputs();
  else
    return 1;
}

NodeDataType MathOperationNodeModel::dataType(PortType, PortIndex) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> MathOperationNodeModel::outData(PortIndex) { return _resultPtr; }

void MathOperationNodeModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    emit dataInvalidated(0);
  }

  _inputNumbers[portIndex] = data;
  compute();
}
