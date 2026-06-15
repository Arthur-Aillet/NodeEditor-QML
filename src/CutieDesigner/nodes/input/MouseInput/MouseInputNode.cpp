#include "MouseInputNode.hpp"
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

MouseInputNode::MouseInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _xPtr(std::make_shared<DecimalData>(_x)),
      _yPtr(std::make_shared<DecimalData>(_y)) {
  auto app = qvariant_cast<QApplication *>(engine->rootContext()->contextProperty("app"));

  _window = getCutieWindow(app);
  if (_window == nullptr)
    return;

  QObject::connect(_window, SIGNAL(mouseMoveEventForward(QMouseEvent *)), this,
                   SLOT(mouseMoveEvent(QMouseEvent *)));
}

unsigned int MouseInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 2;
  default:
    return 0;
  }
}

QString MouseInputNode::portCaption(PortType portType, PortIndex portIndex) const {
  if (portIndex == 0)
    return "x";
  else
    return "y";
}

const NodeDataType &MouseInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> MouseInputNode::outData(PortIndex portIndex) {
  if (portIndex == 0)
    return _xPtr;
  else
    return _yPtr;
}

void MouseInputNode::mouseMoveEvent(QMouseEvent *event) {
  if (event->pos().x() != _x) {
    _x = event->pos().x();
    emit dataUpdated(0);
  }
  if (event->pos().y() != _y) {
    _y = event->pos().y();
    emit dataUpdated(1);
  }
}