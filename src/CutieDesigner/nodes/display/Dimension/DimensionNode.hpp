#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>

class DimensionNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *surface READ getSurface NOTIFY surfaceChanged)
  Q_PROPERTY(double inX READ inX NOTIFY inXChanged)
  Q_PROPERTY(double inY READ inY NOTIFY inYChanged)
  Q_PROPERTY(double inWidth READ inWidth NOTIFY inWidthChanged)
  Q_PROPERTY(double inHeight READ inHeight NOTIFY inHeightChanged)

  DimensionNode(QQmlEngine *engine);
  ~DimensionNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Dimension"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *getSurface() { return _surface.lock().get(); }
  double inX() {
    if (_inX.expired())
      return 0;
    return _inX.lock()->repr<double>();
  }
  double inY() {
    if (_inY.expired())
      return 0;
    return _inY.lock()->repr<double>();
  }
  double inWidth() {
    if (_inWidth.expired())
      return 100;
    return _inWidth.lock()->repr<double>();
  }
  double inHeight() {
    if (_inHeight.expired())
      return 100;
    return _inHeight.lock()->repr<double>();
  }

  signals:
  void surfaceChanged();
  void inXChanged();
  void inYChanged();
  void inWidthChanged();
  void inHeightChanged();

  private:
  std::weak_ptr<SurfaceData> _surface;
  std::weak_ptr<NodeData> _inX;
  std::weak_ptr<NodeData> _inY;
  std::weak_ptr<NodeData> _inWidth;
  std::weak_ptr<NodeData> _inHeight;
  std::shared_ptr<SurfaceData> _content;
};
