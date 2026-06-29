#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

class CosNode : public MathOperationNodeModel {
  public:
  CosNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  ~CosNode() = default;

  public:
  QString caption() const override { return "cos"; }

  unsigned int nbInputs() const override { return 1; };
  QString name() const override { return "Cos"; }

  private:
  void compute() override {
    auto n1 = _inputNumbers[0].lock();

    if (n1) {
      _result = cos(n1->repr<double>());
      _resultPtr = std::make_shared<DecimalData>();
    } else {
      _resultPtr.reset();
    }

    emit dataUpdated(0);
  }
};
