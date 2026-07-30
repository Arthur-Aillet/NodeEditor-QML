#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QUrl>
#include <optional>

class VideoDisplayNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QUrl source READ getSource WRITE setSource NOTIFY sourceChanged)
  Q_PROPERTY(QString sourceFileName READ getSourceFileName NOTIFY sourceChanged)

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

  QUrl getSource();
  void setSource(QUrl url);
  QString getSourceFileName();

  signals:
  void sourceChanged();

  private:
  std::optional<QUrl> _sourceUrl = std::nullopt;
  std::shared_ptr<SurfaceData> _content = nullptr;
};
