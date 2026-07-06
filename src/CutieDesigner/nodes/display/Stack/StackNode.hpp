#pragma once

#include "Definitions.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QAbstractItemModel>

class SurfaceList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int portsCount() const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  void setPort(std::shared_ptr<SurfaceData> surface, int portIndex);
  void addEmptyPort();
  void removeLastPort();

  protected:
  std::vector<std::weak_ptr<SurfaceData>> _surfaces = {{}};
};

class StackNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceList *surfaceList READ getSurfaceList CONSTANT)

  StackNode(QQmlEngine *engine);
  ~StackNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Stack"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "StackControl");
  }

  QVariantMap componentInitialProperties() override {
    QVariantMap map;
    map["node"] = QVariant::fromValue(this);
    return map;
  }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  Q_INVOKABLE void addEmptyPort();
  Q_INVOKABLE void removeLastPort();

  SurfaceList *getSurfaceList() { return &_surfaceList; }

  private:
  SurfaceList _surfaceList;
  std::shared_ptr<SurfaceData> _content;
};
