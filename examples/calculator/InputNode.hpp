#pragma once

#include <NodeEditor/NodeModel>

#include <memory>

#include <QtQmlIntegration>

class DecimalData;

class InputNode : public NodeEditor::NodeModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  public:
  Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

  InputNode(QQmlEngine *engine);
  ~InputNode() = default;

  QString caption() const override;
  QString name() const override;

  QJsonObject save() const override;
  void load(QJsonObject const &p) override;

  unsigned int nPorts(NodeEditor::PortSide portSide) const override;

  NodeEditor::NodeDataType dataType(NodeEditor::PortSide portSide,
                                    NodeEditor::PortIndex portIndex) const override;
  bool portCaptionVisible(NodeEditor::PortSide portSide,
                          NodeEditor::PortIndex portIndex) const override;
  QString portCaption(NodeEditor::PortSide portSide,
                      NodeEditor::PortIndex portIndex) const override;

  std::shared_ptr<NodeEditor::NodeData> outData(NodeEditor::PortIndex port) override;
  void setInData(std::shared_ptr<NodeEditor::NodeData> data,
                 NodeEditor::PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;

  double value();
  void setValue(double newValue);

  signals:
  void valueChanged();

  protected:
  double _value;
  std::shared_ptr<DecimalData> _valuePtr;
};
