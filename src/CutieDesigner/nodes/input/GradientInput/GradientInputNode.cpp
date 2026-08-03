#include "GradientInputNode.hpp"
#include "GradientData.hpp"
#include "GradientInputList.hpp"
#include <memory>
#include <qobject.h>

GradientInputNode::GradientInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _list(std::make_shared<GradientInputList>()),
      _content(std::make_shared<GradientData>(_list->gradient)) {
  QObject::connect(
      _list.get(), &GradientInputList::gradientChanged, this, [this]() { emit dataUpdated(0); },
      Qt::AutoConnection);
}

unsigned int GradientInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType GradientInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return GradientData().type();
}

std::shared_ptr<NodeData> GradientInputNode::outData(PortIndex _portIndex) { return _content; }
