#pragma once

#include "DecimalData.hpp"
#include "MathOperationNodeModel.hpp"

#include <QtCore/QObject>

class MinNode : public MathOperationNodeModel {
  public:
  MinNode(QQmlEngine *engine) : MathOperationNodeModel(engine) {}
  virtual ~MinNode() {}

  public:
  QString caption() const override { return "min"; }

  bool portCaptionVisible(PortType _portType, PortIndex _portIndex) const override { return true; }

  QString name() const override { return "Min"; }

  private:
  void compute() override {
    PortIndex const outPortIndex = 0;

    auto n1 = _inputNumbers[0].lock();
    auto n2 = _inputNumbers[1].lock();

    if (n1 && n2) {
      _result = std::min(n1->repr<double>(), n2->repr<double>());
      _resultPtr = std::make_shared<DecimalData>(_result);
    } else {
      _resultPtr.reset();
    }

    Q_EMIT dataUpdated(outPortIndex);
  }
};
