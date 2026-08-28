#pragma once

#include "SimpleData.hpp"

#include <NodeEditor/NodeModel>

#include <QtQmlIntegration>

using namespace NodeEditor;

class InNode : public NodeEditor::NodeModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  public:
  InNode(QQmlEngine *engine) : NodeModel(engine) {}
  ~InNode() = default;

  QString caption() const override { return "in"; };
  QString name() const override { return "In"; };

  unsigned int nPorts(PortSide portSide) const override { return portSide == PortSide::Out; };

  NodeDataType dataType(PortSide portSide, PortIndex portIndex) const override {
    return SimpleData().type();
  };

  std::shared_ptr<NodeData> outData(PortIndex port) override { return nullptr; }
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override {}
};
