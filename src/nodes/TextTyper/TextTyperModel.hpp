#pragma once

#include "NodeDelegateModel.hpp"
#include "TextData.hpp"
#include "TextTyperEventList.hpp"
#include <QTimer>

class TextTyperModel : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  Q_PROPERTY(bool play READ getPlay WRITE setPlay NOTIFY playChanged)
  Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
  Q_PROPERTY(TextTyperEventList *model READ getModel CONSTANT)

  TextTyperModel()
      : _content(std::make_shared<TextData>(QString("..."))),
        _timer(std::make_unique<QTimer>(this)) {
    _timer->connect(_timer.get(), SIGNAL(timeout()), this, SLOT(processEvent()));
    _timer->setInterval(100);
    _timer->setSingleShot(true);
  };

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

  TextTyperEventList *getModel() { return &_eventList; }

  bool getPlay() { return _playing; }
  void setPlay(bool playState) {
    _playing = playState;
    if (playState == true) {
      _timer->start();
      _currentEvent = 0;
      // qDebug() << _eventList.events[0];
    } else {
      _timer->stop();
    }
  }

  QString getText() { return _content->text; }
  void setText(QString newText) {
    _content->text = newText;
    Q_EMIT dataUpdated(0);
  }

  signals:
  void textChanged();
  void playChanged();

  public slots:
  void processEvent() {
    _content->text.append('.');
    Q_EMIT textChanged();
    Q_EMIT dataUpdated(0);
  }

  private:
  int _currentEvent;
  std::shared_ptr<QQuickItem> _textTyperQml{nullptr};
  TextTyperEventList _eventList;
  std::unique_ptr<QTimer> _timer;
  bool _playing;
  std::shared_ptr<TextData> _content;
};