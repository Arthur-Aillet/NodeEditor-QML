#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <QtCore/QObject>

class SinNode : public MathOperationNodeModel {
  public:
  SinNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  ~SinNode() = default;

  public:
  QString caption() const override { return QStringLiteral("sin"); }

  unsigned int nbInputs() const override { return 1; };
  QString name() const override { return QStringLiteral("Sin"); }

  private:
  void compute() override {
    auto n1 = _inputNumbers[0].lock();

    if (n1) {
      _result = std::make_shared<DecimalData>(sin(n1->number));
    } else {
      _result.reset();
    }

    emit dataUpdated(0);
  }
};
