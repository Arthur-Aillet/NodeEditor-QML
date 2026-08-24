#pragma once

#include "Definitions.hpp"
#include "NodeDelegateModel.hpp"
#include <memory>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class DecimalData;

class DisplayNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  public:
  Q_PROPERTY(double value READ value NOTIFY valueChanged)

  DisplayNode(QQmlEngine *engine);
  ~DisplayNode() = default;

  public:
  QString name() const override;
  unsigned int nPorts(PortSide portSide) const override;

  NodeDataType dataType(PortSide portSide, PortIndex portIndex) const override;
  bool portCaptionVisible(PortSide portSide, PortIndex portIndex) const override;
  QString portCaption(PortSide portSide, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;

  double value();

  signals:
  void valueChanged();

  protected:
  std::weak_ptr<NodeData> _valuePtr;
};
