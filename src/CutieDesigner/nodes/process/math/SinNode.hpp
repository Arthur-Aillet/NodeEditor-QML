#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

class SinNode : public MathOperationNodeModel {
  public:
  SinNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  ~SinNode() = default;

  public:
  QString caption() const override { return "sin"; }

  unsigned int nbInputs() const override { return 1; };
  QString name() const override { return "Sin"; }

  private:
  void compute() override {
    auto n1 = _inputNumbers[0].lock();

    if (n1) {
      _result = sin(n1->repr<double>());
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      _resultPtr.reset();
    }

    emit dataUpdated(0);
  }
};
