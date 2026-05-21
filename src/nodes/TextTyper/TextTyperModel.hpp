#pragma once

#include "NodeDelegateModel.hpp"
#include "TextTyperEventList.hpp"

class TextTyperModel : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  Q_PROPERTY(bool play MEMBER _playing NOTIFY playChanged)
  TextTyperModel() : eventList(std::make_shared<TextTyperEventList>()) {};
  ~TextTyperModel() = default;

  QString caption() const override { return QStringLiteral("Text Typer"); }

  bool captionVisible() const override { return true; }

  QString name() const override { return QStringLiteral("Text Typer"); }

  QString portCaption(PortType type, PortIndex index) const override;

  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  public:
  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  void embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) override;
  QVariantMap componentInitialProperties() override;

  signals:
  void playChanged();

  private:
  std::shared_ptr<QQuickItem> _textTyperQml{nullptr};
  std::shared_ptr<TextTyperEventList> eventList;

  bool _playing;
  QString _content;
};