#include "CameraNode.hpp"

#include <QtWidgets/QLabel>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qquickitem.h>
#include <qtimer.h>
#include <qtmetamacros.h>

CameraNode::CameraNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _window = CutieWindow::getCutieWindow(engine);
  _engine = engine;

  if (!_window)
    return;

  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(_engine, "CutieDesigner.Nodes.Display", "Camera"),
      QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int CameraNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType CameraNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> CameraNode::outData(PortIndex _portIndex) { return _content; }

void CameraNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString CameraNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return "";
  default:
    return "out";
  }
}