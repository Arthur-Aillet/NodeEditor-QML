#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class SurfaceDisplayNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  SurfaceDisplayNode(QQmlEngine *engine);
  Q_PROPERTY(SurfaceData *content READ getContent NOTIFY contentChanged)

  ~SurfaceDisplayNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Result"); }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *getContent() { return _content.lock().get(); }

  signals:
  void contentChanged(SurfaceData *);

  private:
  std::weak_ptr<SurfaceData> _content;
};
