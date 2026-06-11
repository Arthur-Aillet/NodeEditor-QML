#pragma once

#include "Definitions.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class StackNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(int portCount READ portCount WRITE setPortCount NOTIFY portCountChanged)

  StackNode(QQmlEngine *engine);
  ~StackNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Stack"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes", "StackControl");
  }

  QVariantMap componentInitialProperties() override {
    QVariantMap map;
    map["node"] = QVariant::fromValue(this);
    return map;
  }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
  int portCount() { return _portCount; }
  void setPortCount(int portCount) {
    if (_portCount == portCount || portCount < 1)
      return;
    if (_portCount > portCount) {
      emit portsAboutToBeDeleted(PortType::In, portCount, _portCount);
      _portCount = portCount;
      emit portsDeleted(PortType::In);
    } else {
      emit portsAboutToBeInserted(PortType::In, _portCount, portCount);
      _portCount = portCount;
      emit portsInserted(PortType::In);
    }
    emit portCountChanged();
  }

  signals:
  void portCountChanged();

  private:
  uint _portCount = 1;
  std::shared_ptr<SurfaceData> _content;
};
