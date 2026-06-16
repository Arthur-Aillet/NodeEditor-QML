#pragma once

#include "CutieWindow.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QQuickItem>
#include <QtCore/QObject>
#include <qforeach.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class MouseInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  MouseInputNode(QQmlEngine *engine);
  ~MouseInputNode() override {
    if (_window != nullptr) {
      _window->stopRequestRefresh(this);
    }
  };

  bool captionVisible() const override { return true; }
  QString name() const override { return "Mouse"; }

  bool labelVisible() const override { return true; }
  QString label() const override { return "Mouse pos in window space"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void mouseMoveEvent(QMouseEvent *event);

  private:
  double _x = 0.0;
  double _y = 0.0;
  CutieWindow *_window = nullptr;
  std::shared_ptr<DecimalData> _xPtr;
  std::shared_ptr<DecimalData> _yPtr;
};