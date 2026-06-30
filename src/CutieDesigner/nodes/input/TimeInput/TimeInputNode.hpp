#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"
#include <memory>

class TimeInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  TimeInputNode(QQmlEngine *engine);
  ~TimeInputNode() override = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Time"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType _portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  protected:
  std::array<double, 5> _outContent;
  std::array<std::shared_ptr<DecimalData>, 5> _outData;
};