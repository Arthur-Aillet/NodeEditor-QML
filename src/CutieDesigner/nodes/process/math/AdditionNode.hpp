#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <QtCore/QObject>

class AdditionNode : public MathOperationNodeModel {
  public:
  AdditionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  ~AdditionNode() = default;

  public:
  QString caption() const override { return QStringLiteral("add"); }
  bool labelVisible() const override { return true; }

  QString name() const override { return QStringLiteral("Addition"); }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      _result = std::make_shared<DecimalData>(n1->number + n2->number);
    } else {
      _result.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
