#pragma once

#include "SimpleData.hpp"

#include <NodeEditor/NodeModel>

#include <QtQmlIntegration>

using namespace NodeEditor;

class OutNode : public NodeEditor::NodeModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  public:
  OutNode(QQmlEngine *engine) : NodeModel(engine) {}
  ~OutNode() = default;

  QString caption() const override { return "out"; };
  QString name() const override { return "Out"; };

  unsigned int nPorts(PortSide portSide) const override { return portSide == PortSide::In; };

  NodeDataType dataType(PortSide portSide, PortIndex portIndex) const override {
    return SimpleData().type();
  };

  std::shared_ptr<NodeData> outData(PortIndex port) override { return nullptr; }
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override {}
};
