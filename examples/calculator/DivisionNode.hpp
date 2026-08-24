#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <QtCore/QObject>

class DivisionNode : public MathOperationNodeModel {
  public:
  DivisionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~DivisionNode() {}

  public:
  QString caption() const override { return "div"; }

  bool portCaptionVisible(PortSide _portSide, PortIndex _portIndex) const override { return true; }

  QString name() const override { return "Division"; }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    NodeValidationState state;
    if (n2 && (n2->repr<double>() == 0.0)) {
      state._state = NodeValidationState::State::Error;
      state._stateMessage = QStringLiteral("Division by zero error");
      setValidationState(state);
      _resultPtr.reset();
    } else if (n2 && (n2->repr<double>() < 1e-5)) {
      state._state = NodeValidationState::State::Warning;
      state._stateMessage = QStringLiteral("Very small divident. Result might overflow");
      setValidationState(state);
      if (n1) {
        _result = n1->repr<double>() / n2->repr<double>();
        _resultPtr = std::make_shared<DecimalData>(_result);
      } else {
        _resultPtr.reset();
      }
    } else if (n1 && n2) {
      setValidationState(state);
      _result = n1->repr<double>() / n2->repr<double>();
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      NodeValidationState state;
      setValidationState(state);
      _resultPtr.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
