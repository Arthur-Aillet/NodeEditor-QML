#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

class AdditionNode : public MathOperationNodeModel {
  public:
  AdditionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  ~AdditionNode() = default;

  public:
  QString caption() const override { return "add"; }
  QString name() const override { return "Addition"; }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      _result = n1->repr<double>() + n2->repr<double>();
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      _resultPtr.reset();
    }

    emit dataUpdated(outPortIndex);
  }
};
