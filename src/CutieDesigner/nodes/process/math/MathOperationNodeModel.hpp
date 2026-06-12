#pragma once

#include "Definitions.hpp"
#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qvariant.h>

class DecimalData;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationNodeModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  MathOperationNodeModel(QQmlEngine *engine)
      : NodeDelegateModel(engine), _inputNumbers(nbInputs()) {}
  ~MathOperationNodeModel() = default;

  public:
  virtual unsigned int nbInputs() const { return 2; };
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  virtual bool portCaptionVisible(PortType, PortIndex) const override { return true; }
  virtual QString portCaption(PortType portType, PortIndex portIndex) const override {
    if (portType == PortType::Out)
      return "out";
    if (nbInputs() == 1)
      return "in";
    return QString((char)('a' + portIndex));
  }

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  protected:
  virtual void compute() = 0;

  protected:
  std::vector<std::weak_ptr<NodeData>> _inputNumbers;

  QProperty<QVariant> _result;
  std::shared_ptr<DecimalData> _resultPtr;
};
