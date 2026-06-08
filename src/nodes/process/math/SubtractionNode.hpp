#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

class SubtractionNode : public MathOperationNodeModel {
  public:
  SubtractionNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~SubtractionNode() {}

  public:
  QString caption() const override { return QStringLiteral("sub"); }

  virtual bool portCaptionVisible(PortType portType, PortIndex portIndex) const override {
    Q_UNUSED(portType);
    Q_UNUSED(portIndex);
    return true;
  }

  QString name() const override { return QStringLiteral("Subtraction"); }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      _result = std::make_shared<DecimalData>(n1->number() - n2->number());
    } else {
      _result.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
