#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

class SubtractionNode : public MathOperationNodeModel {
  public:
  SubtractionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~SubtractionNode() {}

  public:
  QString caption() const override { return "sub"; }

  virtual bool portCaptionVisible(PortType portType, PortIndex portIndex) const override {
    Q_UNUSED(portType);
    Q_UNUSED(portIndex);
    return true;
  }

  QString name() const override { return "Subtraction"; }

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
