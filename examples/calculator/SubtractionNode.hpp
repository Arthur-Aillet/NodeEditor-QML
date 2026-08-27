#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <NodeEditor/NodeModel>

class SubtractionNode : public MathOperationNodeModel {
  public:
  SubtractionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~SubtractionNode() {}

  public:
  QString caption() const override { return "sub"; }
  QString name() const override { return "Subtraction"; }

  virtual bool portCaptionVisible(PortSide portSide, PortIndex portIndex) const override {
    Q_UNUSED(portSide);
    Q_UNUSED(portIndex);
    return true;
  }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      _result = n1->repr<double>() - n2->repr<double>();
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      _resultPtr.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
