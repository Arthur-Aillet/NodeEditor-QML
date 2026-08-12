#include "MathOperationNodeModel.hpp"
#include "DecimalData.hpp"

unsigned int MathOperationNodeModel::nPorts(PortSide portSide) const {
  if (portSide == PortSide::In)
    return nbInputs();
  else
    return 1;
}

NodeDataType MathOperationNodeModel::dataType(PortSide _portSide, PortIndex _portIndex) const {
  return DecimalData().type();
}

bool MathOperationNodeModel::portCaptionVisible(PortSide _portSide, PortIndex _portIndex) const {
  return true;
}

QString MathOperationNodeModel::portCaption(PortSide portSide, PortIndex portIndex) const {
  if (portSide == PortSide::Out)
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
