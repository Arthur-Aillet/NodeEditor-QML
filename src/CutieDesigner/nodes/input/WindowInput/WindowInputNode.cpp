#include "WindowInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qapplication.h>
#include <qevent.h>
#include <qlogging.h>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qqmlcontext.h>
#include <qquickwindow.h>
#include <qvalidator.h>
#include <qvariant.h>
#include <qwindow.h>

static CutieWindow *getCutieWindow(QApplication *application) {
  for (auto w : application->allWindows()) {
    CutieWindow *cutieWindow = qobject_cast<CutieWindow *>(w);
    if (cutieWindow != nullptr)
      return cutieWindow;
  }
  return nullptr;
}

WindowInputNode::WindowInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _widthPtr(std::make_shared<DecimalData>(_width)),
      _heightPtr(std::make_shared<DecimalData>(_height)) {
  auto app = qvariant_cast<QApplication *>(engine->rootContext()->contextProperty("app"));

  _window = getCutieWindow(app);
  if (_window == nullptr)
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

void WindowInputNode::resizeEvent(QResizeEvent *event) {
  if (event->size().width() != _width) {
    _width = event->size().width();
    emit dataUpdated(0);
  }
  if (event->size().height() != _height) {
    _height = event->size().height();
    emit dataUpdated(1);
  }
}