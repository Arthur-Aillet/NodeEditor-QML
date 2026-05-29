#include "ObjectDisplayDataModel.hpp"
#include "NodeDelegateModel.hpp"
#include "ObjectData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

ObjectDisplayDataModel::ObjectDisplayDataModel(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(nullptr) {}

unsigned int ObjectDisplayDataModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &ObjectDisplayDataModel::dataType(PortType, PortIndex) const {
  return ObjectData().type();
}

std::shared_ptr<NodeData> ObjectDisplayDataModel::outData(PortIndex) { return nullptr; }

void ObjectDisplayDataModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    _content = nullptr;
    emit componentRemoved();
  } else {
    _content = std::dynamic_pointer_cast<ObjectData>(data);
    emit componentSet(_content.get());
  }
}
