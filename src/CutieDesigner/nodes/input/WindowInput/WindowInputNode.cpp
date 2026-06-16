#include "WindowInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qapplication.h>
#include <qevent.h>
#include <qlogging.h>
#include <qqmlcomponent.h>
#include <qqmlcontext.h>
#include <qquickwindow.h>
#include <qvalidator.h>
#include <qvariant.h>
#include <qwindow.h>

WindowInputNode::WindowInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _widthPtr(std::make_shared<DecimalData>(_width)),
      _heightPtr(std::make_shared<DecimalData>(_height)) {
  auto window = CutieWindow::getCutieWindow(engine);
  if (window == nullptr)
    return;

  _width = _window->width();
  _height = _window->height();

  QObject::connect(_window, SIGNAL(resizeEventForward(QResizeEvent *)), this,
                   SLOT(resizeEvent(QResizeEvent *)));
}

unsigned int WindowInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 2;
  default:
    return 0;
  }
}

QString WindowInputNode::portCaption(PortType portType, PortIndex portIndex) const {
  if (portIndex == 0)
    return "width";
  else
    return "height";
}

const NodeDataType &WindowInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> WindowInputNode::outData(PortIndex portIndex) {
  if (portIndex == 0)
    return _widthPtr;
  else
    return _heightPtr;
}

void WindowInputNode::resizeEvent(QResizeEvent *_event) {
  auto frame = qvariant_cast<QQuickItem *>(_window->property("sceneContent"));

  if (frame->size().width() != _width) {
    _width = frame->size().width();
    emit dataUpdated(0);
  }
  if (frame->size().height() != _height) {
    _height = frame->size().height();
    emit dataUpdated(1);
  }

  // if (event->size().width() != _width) {
  //   _width = event->size().width();
  //   emit dataUpdated(0);
  // }
  // if (event->size().height() != _height) {
  //   _height = event->size().height();
  //   emit dataUpdated(1);
  // }
}