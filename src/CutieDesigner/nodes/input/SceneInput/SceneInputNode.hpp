#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QQuickItem>
#include <QtCore/QObject>
#include <qevent.h>
#include <qforeach.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qquickitem.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class SceneInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  SceneInputNode(QQmlEngine *engine);
  ~SceneInputNode() override = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Scene"; }

  bool labelVisible() const override { return true; }
  QString label() const override { return "Scene dimensions"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void xChanged();
  void yChanged();
  void widthChanged();
  void heightChanged();

  private:
  double _x = 0.0;
  double _y = 0.0;
  double _width = 0.0;
  double _height = 0.0;
  QQuickItem *_frame = nullptr;
  std::shared_ptr<DecimalData> _xPtr;
  std::shared_ptr<DecimalData> _yPtr;
  std::shared_ptr<DecimalData> _widthPtr;
  std::shared_ptr<DecimalData> _heightPtr;
};