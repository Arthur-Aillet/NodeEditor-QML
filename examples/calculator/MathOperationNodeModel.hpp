#pragma once

#include "Definitions.hpp"
#include "NodeDelegateModel.hpp"

class DecimalData;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationNodeModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  MathOperationNodeModel(QQmlEngine *engine)
      : NodeDelegateModel(engine), _inputNumbers(nbInputs()) {}
  ~MathOperationNodeModel() = default;

  public:
  virtual unsigned int nbInputs() const { return 2; };
  unsigned int nPorts(PortSide portSide) const override;

  NodeDataType dataType(PortSide portSide, PortIndex portIndex) const override;
  virtual bool portCaptionVisible(PortSide portSide, PortIndex portIndex) const override;
  virtual QString portCaption(PortSide portSide, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex portIndex) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  protected:
  virtual void compute() = 0;

  protected:
  std::vector<std::weak_ptr<NodeData>> _inputNumbers;

  double _result;
  std::shared_ptr<DecimalData> _resultPtr;
};
