#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qvectornd.h>

class DimensionNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *surface READ getSurface NOTIFY surfaceChanged)
  Q_PROPERTY(QVector2D inPos READ inPos NOTIFY inPosChanged)
  Q_PROPERTY(QVector2D inSize READ inSize NOTIFY inSizeChanged)
  Q_PROPERTY(double rotation READ rotation NOTIFY rotationChanged)

  DimensionNode(QQmlEngine *engine);
  ~DimensionNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Dimension"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *getSurface() { return _surface.lock().get(); }
  QVector2D inPos() {
    if (_inPos.expired())
      return QVector2D(0, 0);
    return _inPos.lock()->repr<QVector2D>();
  }
  QVector2D inSize() {
    if (_inSize.expired())
      return QVector2D(200, 200);
    return _inSize.lock()->repr<QVector2D>();
  }
  double rotation() {
    if (_rotation.expired())
      return 0;
    return _rotation.lock()->repr<double>();
  }

  signals:
  void surfaceChanged();
  void inPosChanged();
  void inSizeChanged();
  void rotationChanged();

  private:
  std::weak_ptr<SurfaceData> _surface;
  std::weak_ptr<NodeData> _inPos;
  std::weak_ptr<NodeData> _inSize;
  std::weak_ptr<NodeData> _rotation;
  std::shared_ptr<SurfaceData> _content;
};
