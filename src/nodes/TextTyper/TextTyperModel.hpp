#pragma once

#include "NodeDelegateModel.hpp"
#include "TextData.hpp"
#include "TextTyperEventList.hpp"
#include <QTimer>
#include <qnamespace.h>
#include <qtmetamacros.h>

class TextTyperModel : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  Q_PROPERTY(bool play READ getPlay WRITE setPlay NOTIFY playChanged)
  Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
  Q_PROPERTY(TextTyperEventList *model READ getModel CONSTANT)

  TextTyperModel() : _content(std::make_shared<TextData>(QString("..."))), _timer(QTimer()){};

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

  TextTyperEventList *getModel();
  bool getPlay();
  void setPlay(bool playState);
  QString getText();
  void setText(QString newText);

  void processEvent();

  signals:
  void textChanged();
  void playChanged();

  protected slots:
  void processNextEvent();
  void processWait();
  void processErase();
  void processReplace();
  void processInsert();

  private:
  int _currentEventIdx = 0;
  TextTyperEventList::TypeEvent _currentEvent;
  std::shared_ptr<QQuickItem> _textTyperQml{nullptr};
  TextTyperEventList _eventList;
  QTimer _timer;
  bool _playing;
  std::shared_ptr<TextData> _content;
};