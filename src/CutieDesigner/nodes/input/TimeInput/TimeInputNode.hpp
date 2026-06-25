#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

class TimeInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  TimeInputNode(QQmlEngine *engine);
  ~TimeInputNode() override = default;

  bool captionVisible() const override { return false; }
  QString name() const override { return "Time"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return "Time"; }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void updateTime();

  private:
  double _number = 0.0;
  std::shared_ptr<QQuickItem> _portLabel{nullptr};
  std::shared_ptr<DecimalData> _numberData;
};