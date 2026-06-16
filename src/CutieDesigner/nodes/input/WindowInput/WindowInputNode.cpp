#include "WindowInputNode.hpp"

WindowInputNode::WindowInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _sizeData(std::make_shared<Vec2Data>(_size)) {
  _window = CutieWindow::getCutieWindow(engine);
  if (_window == nullptr)
    return;

  _size = QVector2D(_window->width(), _window->height());

  QObject::connect(_window, SIGNAL(resizeEventForward(QResizeEvent *)), this,
                   SLOT(resizeEvent(QResizeEvent *)));
}

unsigned int WindowInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QString WindowInputNode::portCaption(PortType portType, PortIndex portIndex) const {
  return "size";
}

const NodeDataType &WindowInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return Vec2Data().type();
}

std::shared_ptr<NodeData> WindowInputNode::outData(PortIndex portIndex) { return _sizeData; }

void WindowInputNode::resizeEvent(QResizeEvent *_event) {
  auto frame = qvariant_cast<QQuickItem *>(_window->property("sceneContent"));

  if (QVector2D(frame->width(), frame->height()) != _size) {
    _size = QVector2D(frame->width(), frame->height());
    emit dataUpdated(0);
  }
}