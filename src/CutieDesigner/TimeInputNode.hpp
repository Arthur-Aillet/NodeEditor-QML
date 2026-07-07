#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"
#include <qtmetamacros.h>

class TimeInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  TimeInputNode(QQmlEngine *engine)
      : NodeDelegateModel(engine), _outContent(2),
        _outData(std::make_shared<DecimalData>(_outContent)) {}

  ~TimeInputNode() override = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Time"; }

  unsigned int nPorts(PortType portType) const override { return 1; }
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {
    return DecimalData().type();
  }
  std::shared_ptr<NodeData> outData(PortIndex port) override { return _outData; }

  QString portCaption(PortType _portType, PortIndex portIndex) const override { return "time"; }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData> a, PortIndex) override { _inData = a; }

  protected:
  double _outContent;
  std::shared_ptr<DecimalData> _outData;
  std::weak_ptr<NodeData> _inData;
};