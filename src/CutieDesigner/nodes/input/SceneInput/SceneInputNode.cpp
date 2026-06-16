#include "SceneInputNode.hpp"
#include "CutieWindow.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qapplication.h>
#include <qevent.h>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qqmlcontext.h>
#include <qquickitem.h>
#include <qquickwindow.h>
#include <qvalidator.h>
#include <qvariant.h>
#include <qwindow.h>

SceneInputNode::SceneInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _xPtr(std::make_shared<DecimalData>(_x)),
      _yPtr(std::make_shared<DecimalData>(_y)), _widthPtr(std::make_shared<DecimalData>(_width)),
      _heightPtr(std::make_shared<DecimalData>(_height)) {
  auto window = CutieWindow::getCutieWindow(engine);
  if (window == nullptr)
    return;

  _frame = qvariant_cast<QQuickItem *>(window->property("sceneContent"));

  _x = _frame->x();
  _y = _frame->y();
  _width = _frame->width();
  _height = _frame->height();

  QObject::connect(_frame, SIGNAL(xChanged()), this, SLOT(xChanged()));
  QObject::connect(_frame, SIGNAL(yChanged()), this, SLOT(yChanged()));
  QObject::connect(_frame, SIGNAL(widthChanged()), this, SLOT(widthChanged()));
  QObject::connect(_frame, SIGNAL(heightChanged()), this, SLOT(heightChanged()));
}

unsigned int SceneInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 4;
  default:
    return 0;
  }
}

QString SceneInputNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portIndex) {
  case 0:
    return "x";
  case 1:
    return "y";
  case 2:
    return "width";
  default:
    return "height";
  }
}

const NodeDataType &SceneInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> SceneInputNode::outData(PortIndex portIndex) {
  switch (portIndex) {
  case 0:
    return _xPtr;
  case 1:
    return _yPtr;
  case 2:
    return _widthPtr;
  default:
    return _heightPtr;
  }
}

void SceneInputNode::xChanged() {
  if (_frame->x() != _x) {
    _x = _frame->x();
    emit dataUpdated(0);
  }
}

void SceneInputNode::yChanged() {
  if (_frame->y() != _y) {
    _y = _frame->y();
    emit dataUpdated(1);
  }
}

void SceneInputNode::widthChanged() {
  if (_frame->width() != _width) {
    _width = _frame->width();
    emit dataUpdated(2);
  }
}

void SceneInputNode::heightChanged() {
  if (_frame->height() != _height) {
    _height = _frame->height();
    emit dataUpdated(3);
  }
}
