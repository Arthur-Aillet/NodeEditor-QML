#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QUrl>
#include <QVideoSink>
#include <QtAVPlayer/qavplayer.h>
#include <QtMultimediaQuick/private/qquickvideooutput_p.h>
#include <optional>
#include <qtmetamacros.h>

class VideoDisplayNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
  Q_PROPERTY(QString sourceFileName READ sourceFileName NOTIFY sourceChanged)
  Q_PROPERTY(bool looping READ looping WRITE setLooping NOTIFY loopingChanged)
  Q_PROPERTY(double playbackRate READ playbackRate NOTIFY playbackRateChanged)

  VideoDisplayNode(QQmlEngine *engine);
  ~VideoDisplayNode() = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Video"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;

  QUrl source();
  void setSource(QUrl url);
  QString sourceFileName();
  bool looping();
  void setLooping(bool looping);
  double playbackRate();

  Q_INVOKABLE void newVideoOutput(QQuickVideoOutput *output);
  Q_INVOKABLE void removeVideoOutput(QQuickVideoOutput *output);

  signals:
  void sourceChanged();
  void loopingChanged();
  void playbackRateChanged();

  private:
  std::vector<QQuickVideoOutput *> _sinks;
  QAVPlayer _player;
  bool _looping = true;
  std::optional<QUrl> _sourceUrl = std::nullopt;
  double _playbackRate = 1.0;
  std::shared_ptr<SurfaceData> _content = nullptr;
};
