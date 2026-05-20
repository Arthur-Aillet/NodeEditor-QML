#pragma once

#include "NodeDelegateModel.hpp"
#include "TextTyperEventList.hpp"

class TextTyperModel : public NodeDelegateModel {
  Q_OBJECT

  public:
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
  const QUrl embeddedComponent(QQmlEngine *engine) override;
  void embeddedComponentLoaded(QObject *loaded) override;
  QVariantMap componentInitialProperties() override;

  private:
  QObject *_textTyperQml{nullptr};
  std::shared_ptr<TextTyperEventList> eventList;

  QString _content;

  std::shared_ptr<QQmlComponent> _component{nullptr};
};