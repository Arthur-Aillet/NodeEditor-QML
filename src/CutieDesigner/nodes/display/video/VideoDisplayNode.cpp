#include "VideoDisplayNode.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qquickitem.h>
#include <qtimer.h>
#include <qtmetamacros.h>

VideoDisplayNode::VideoDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _window = CutieWindow::getCutieWindow(engine);
  _engine = engine;

  if (!_window)
    return;

  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(_engine, "CutieDesigner.Nodes.Display", "VideoDisplay"),
      QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int VideoDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType VideoDisplayNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> VideoDisplayNode::outData(PortIndex _portIndex) { return _content; }

void VideoDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString VideoDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return "";
  default:
    return "out";
  }
}