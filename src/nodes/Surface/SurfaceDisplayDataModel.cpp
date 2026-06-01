#include "SurfaceDisplayDataModel.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

SurfaceDisplayDataModel::SurfaceDisplayDataModel(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(nullptr) {}

unsigned int SurfaceDisplayDataModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &SurfaceDisplayDataModel::dataType(PortType, PortIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> SurfaceDisplayDataModel::outData(PortIndex) { return nullptr; }

void SurfaceDisplayDataModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    _content = nullptr;
    emit componentRemoved();
  } else {
    _content = std::dynamic_pointer_cast<SurfaceData>(data);
    emit componentSet(_content.get());
  }
}
