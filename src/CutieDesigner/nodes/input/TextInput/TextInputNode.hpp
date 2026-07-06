#pragma once

#include "NodeDelegateModel.hpp"
#include "TextData.hpp"

class TextInputNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

  TextInputNode(QQmlEngine *engine);
  ~TextInputNode() override = default;

  bool captionVisible() const override { return false; }
  QString name() const override { return "Text"; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return ""; }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;
  QString text();
  void setText(QString text);

  signals:
  void textChanged();

  private:
  QString _text = "";
  std::shared_ptr<TextData> _textData;
};