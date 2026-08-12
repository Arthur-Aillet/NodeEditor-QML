#include "MathOperationNodeModel.hpp"
#include "DecimalData.hpp"

unsigned int MathOperationNodeModel::nPorts(PortType portType) const {
  if (portType == PortType::In)
    return nbInputs();
  else
    return 1;
}

NodeDataType MathOperationNodeModel::dataType(PortType _portType, PortIndex _portIndex) const {
  return DecimalData().type();
}

bool MathOperationNodeModel::portCaptionVisible(PortType _portType, PortIndex _portIndex) const {
  return true;
}

QString MathOperationNodeModel::portCaption(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::Out)
    return "out";
  if (nbInputs() == 1)
    return "in";
  return QString((char)('a' + portIndex));
}

std::shared_ptr<NodeData> MathOperationNodeModel::outData(PortIndex) { return _resultPtr; }

void MathOperationNodeModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    emit dataInvalidated(0);
  }

  _inputNumbers[portIndex] = data;
  compute();
}
