#pragma once

#include <NodeEditor/NodeModel>

#include <QtQmlIntegration>

using namespace NodeEditor;

class InNode : public NodeEditor::NodeModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  public:
  InNode(QQmlEngine *engine);
  ~InNode() = default;

  QString caption() const override;
  QString name() const override;

  unsigned int nPorts(PortSide portSide) const override;

  NodeDataType dataType(PortSide portSide, PortIndex portIndex) const override;
  bool portCaptionVisible(PortSide portSide, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
};
