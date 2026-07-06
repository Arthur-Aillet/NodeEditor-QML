#include "SurfaceDisplayNode.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

SurfaceDisplayNode::SurfaceDisplayNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content() {}

unsigned int SurfaceDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 0;
  }
}

NodeDataType SurfaceDisplayNode::dataType(PortType, PortIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> SurfaceDisplayNode::outData(PortIndex) { return nullptr; }

void SurfaceDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    _content.reset();
    emit contentChanged(nullptr);
  } else {
    _content = std::dynamic_pointer_cast<SurfaceData>(data);
    emit contentChanged(_content.lock().get());
  }
}
