#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QGradient>
#include <qdebug.h>
#include <qvectornd.h>

class FillNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(QList<QVariant> gradient READ gradient NOTIFY gradientChanged)
  Q_PROPERTY(QVector2D start READ start NOTIFY startChanged)
  Q_PROPERTY(QVector2D end READ end NOTIFY endChanged)
  Q_PROPERTY(bool endSet READ endSet NOTIFY endChanged)

  FillNode(QQmlEngine *engine);
  ~FillNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Fill"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QList<QVariant> gradient();
  QVector2D start() {
    if (_start.expired()) {
      return QVector2D(0, 0);
    }
    return _start.lock()->repr<QVector2D>();
  }

  QVector2D end() {
    if (_end.expired()) {
      return QVector2D(0, 0);
    }
    return _end.lock()->repr<QVector2D>();
  }

  bool endSet() { return !_end.expired(); }

  signals:
  void gradientChanged();
  void startChanged();
  void endChanged();

  private:
  QGradient _defaultGradient;
  std::weak_ptr<NodeData> _gradient;
  std::weak_ptr<NodeData> _start;
  std::weak_ptr<NodeData> _end;
  std::shared_ptr<SurfaceData> _content;
};
