#pragma once

#include "CutieWindow.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QQuickItem>
#include <QtCore/QObject>
#include <qevent.h>
#include <qforeach.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class WindowInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  WindowInputNode(QQmlEngine *engine);
  ~WindowInputNode() override {
    if (_window != nullptr) {
      _window->stopRequestRefresh(this);
    }
  };

  bool captionVisible() const override { return true; }
  QString name() const override { return "Window"; }
  QString label() const override { return "Cutie window dimensions"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void resizeEvent(QResizeEvent *event);

  private:
  double _width = 0.0;
  double _height = 0.0;
  CutieWindow *_window = nullptr;
  std::shared_ptr<DecimalData> _widthPtr;
  std::shared_ptr<DecimalData> _heightPtr;
};