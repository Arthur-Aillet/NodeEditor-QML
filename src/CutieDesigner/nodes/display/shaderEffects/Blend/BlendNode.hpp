#pragma once

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

class BlendNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *a READ getA NOTIFY aChanged)
  Q_PROPERTY(SurfaceData *b READ getB NOTIFY bChanged)
  Q_PROPERTY(int mode READ getMode WRITE setMode NOTIFY modeChanged)

  BlendNode(QQmlEngine *engine);
  ~BlendNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Blend"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "BlendControl");
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

  SurfaceData *getA() { return _a.lock().get(); }
  SurfaceData *getB() { return _b.lock().get(); }
  int getMode() { return _mode; }
  void setMode(int mode) {
    if (mode == _mode)
      return;
    _mode = mode;
    emit modeChanged();
  }

  signals:
  void aChanged();
  void bChanged();
  void modeChanged();

  private:
  int _mode = 0;
  std::weak_ptr<SurfaceData> _a;
  std::weak_ptr<SurfaceData> _b;
  std::shared_ptr<SurfaceData> _content;
};
