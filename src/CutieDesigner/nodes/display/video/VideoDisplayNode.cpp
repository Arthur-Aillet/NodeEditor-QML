#include "VideoDisplayNode.hpp"
#include "DecimalData.hpp"
#include "QtAVPlayer/qavplayer.h"
#include "SurfaceData.hpp"
#include <qdebug.h>

VideoDisplayNode::VideoDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "VideoDisplay"),
      QVariantMap{{"node", QVariant::fromValue(this)}});

  QObject::connect(
      &_player, &QAVPlayer::mediaStatusChanged, &_player,
      [this](QAVPlayer::MediaStatus status) {
        if (status == QAVPlayer::MediaStatus::EndOfMedia && _looping) {
          _player.seek(0);
          _player.play();
        }
      },
      Qt::DirectConnection);

  QObject::connect(
      &_player, &QAVPlayer::videoFrame, &_player,
      [this](const QAVVideoFrame &frame) {
        // Might download and convert data
        QVideoFrame videoFrame = frame;
        for (auto sink : _sinks) {
          sink->videoSink()->setVideoFrame(videoFrame);
        }
      },
      Qt::DirectConnection);
}

unsigned int VideoDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 1;
  }
}

NodeDataType VideoDisplayNode::dataType(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::In) {
    switch (portIndex) {
    case (0):
      return DecimalData().type();
    default:
      return DecimalData().type();
    }
  } else {
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> VideoDisplayNode::outData(PortIndex _portIndex) { return _content; }

void VideoDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  switch (portIndex) {
  case (0):
  default:
    if (data == nullptr) {
      _playbackRate = 1.0;
    } else {
      _playbackRate = data->repr<double>();
    }
    _player.setSpeed(_playbackRate);
    emit playbackRateChanged();
  }
}

QString VideoDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return "Playback Rate";
  default:
    return "out";
  }
}

QQmlComponent VideoDisplayNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "VideoDisplayControl");
};
QVariantMap VideoDisplayNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
};

void VideoDisplayNode::newVideoOutput(QQuickVideoOutput *output) { _sinks.push_back(output); }

void VideoDisplayNode::removeVideoOutput(QQuickVideoOutput *output) {
  _sinks.erase(std::remove_if(_sinks.begin(), _sinks.end(),
                              [&output](const QQuickVideoOutput *sink) {
                                return sink == output; // put your condition here
                              }),
               _sinks.end());
}

QUrl VideoDisplayNode::source() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value();
  } else {
    return QUrl();
  }
}

void VideoDisplayNode::setSource(QUrl url) {
  if (_sourceUrl == url)
    return;
  _sourceUrl = url;
  _player.setSource(_sourceUrl->path());
  _player.play();
  emit sourceChanged();
}

QString VideoDisplayNode::sourceFileName() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value().fileName();
  } else {
    return "";
  }
}

bool VideoDisplayNode::looping() { return _looping; }
void VideoDisplayNode::setLooping(bool looping) {
  if (looping == _looping)
    return;
  _looping = looping;
  emit loopingChanged();
}

double VideoDisplayNode::playbackRate() { return _playbackRate; }