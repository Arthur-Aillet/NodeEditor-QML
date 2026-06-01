#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class SurfaceDisplayNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  SurfaceDisplayNode(QQmlEngine *engine);

  ~SurfaceDisplayNode() = default;

  public:
  QString caption() const override { return QStringLiteral("Result"); }
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Result"); }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  signals:
  void componentSet(SurfaceData *component);
  void componentRemoved();

  private:
  bool _connected = false;

  std::shared_ptr<SurfaceData> _content;
};
