#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <QtCore/QObject>

class ModNode : public MathOperationNodeModel {
  public:
  ModNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~ModNode() {}

  public:
  QString caption() const override { return "mod"; }

  bool portCaptionVisible(PortType _portType, PortIndex _portIndex) const override { return true; }

  QString name() const override { return "Mod"; }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    NodeValidationState state;
    if (n2 && (n2->repr<double>() == 0.0)) {
      state._state = NodeValidationState::State::Error;
      state._stateMessage = QStringLiteral("Mod by zero error");
      setValidationState(state);
      _resultPtr.reset();
    } else if (n1 && n2) {
      setValidationState(state);
      _result = std::fmod(n1->repr<double>(), n2->repr<double>());
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      NodeValidationState state;
      setValidationState(state);
      _resultPtr.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
