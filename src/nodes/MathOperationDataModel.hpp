#pragma once

#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <qqmlcomponent.h>
#include <qqmlengine.h>

class DecimalData;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationDataModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  ~MathOperationDataModel() = default;

  public:
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  std::shared_ptr<QQmlComponent> embeddedComponent(QQmlEngine *engine) override { return nullptr; }

  protected:
  virtual void compute() = 0;

  protected:
  std::weak_ptr<DecimalData> _number1;
  std::weak_ptr<DecimalData> _number2;

  std::shared_ptr<DecimalData> _result;
};
