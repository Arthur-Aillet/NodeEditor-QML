#pragma once

#include <QtCore/QObject>

#include "MathOperationNodeModel.hpp"

#include "DecimalData.hpp"

class MultiplicationNode : public MathOperationNodeModel {
  public:
  MultiplicationNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~MultiplicationNode() {}

  public:
  QString caption() const override { return QStringLiteral("mult"); }

  QString name() const override { return QStringLiteral("Multiplication"); }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      // modelValidationState = NodeValidationState::Valid;
      // modelValidationError = QString();
      _result = n1->repr<double>() * n2->repr<double>();
      _resultPtr = std::make_shared<DecimalData>(&_result);
    } else {
      // modelValidationState = NodeValidationState::Warning;
      // modelValidationError = QStringLiteral("Missing or incorrect inputs");
      _resultPtr.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
