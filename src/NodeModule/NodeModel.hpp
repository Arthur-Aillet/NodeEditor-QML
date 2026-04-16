#pragma once

#include "NodeData.hpp"
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

class AdditionNode : public QtNodes::NodeDelegateModel {
  Q_OBJECT
  public:
  // Identity
  QString caption() const override { return "Add"; }
  QString name() const override { return "Addition"; }

  // Ports
  unsigned int nPorts(PortType type) const override { return type == PortType::In ? 2 : 1; }

  NodeDataType dataType(PortType, PortIndex) const override { return NumberData{}.type(); }

  // Data handling
  void setInData(std::shared_ptr<NodeData> data, PortIndex port) override {
    qDebug() << "setInData";
  }

  std::shared_ptr<NodeData> outData(PortIndex port) override { return _result; }

  // Widget (optional)
  QWidget *embeddedWidget() override { return nullptr; }

  private:
  std::shared_ptr<NumberData> _input1, _input2, _result;
};
