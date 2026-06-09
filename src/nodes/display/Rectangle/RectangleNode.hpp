#pragma once

#include "ColorData.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qcolor.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class RectangleNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QColor color READ getColor NOTIFY colorChanged)

  RectangleNode(QQmlEngine *engine);
  ~RectangleNode() = default;

  public:
  QString caption() const override { return QStringLiteral("Rectangle"); }
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Rectangle"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QColor getColor() { return _color.lock()->color; }

  signals:
  void colorChanged();

  private:
  std::weak_ptr<ColorData> _color;
  std::shared_ptr<SurfaceData> _content;
};
