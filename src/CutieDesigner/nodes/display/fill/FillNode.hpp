#pragma once

#include "GradientData.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qbrush.h>
#include <qcolor.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class FillNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QList<QVariant> gradient READ getGradient NOTIFY gradientChanged)

  FillNode(QQmlEngine *engine);
  ~FillNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Fill"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QList<QVariant> getGradient() {
    auto gradient = _gradient.expired() ? _defaultGradient : _gradient.lock()->gradient();
    QList<QVariant> newList;

    for (auto &stop : gradient.stops()) {
      QList<QVariant> newStop;
      newStop.append(QVariant::fromValue(stop.first));
      newStop.append(QVariant::fromValue(stop.second));
      newList.append(QVariant::fromValue(newStop));
    }

    return newList;
  }

  signals:
  void gradientChanged();

  private:
  QGradient _defaultGradient;
  std::weak_ptr<GradientData> _gradient;
  std::shared_ptr<SurfaceData> _content;
};
