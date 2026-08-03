#include "FillNode.hpp"
#include "GradientData.hpp"
#include "SurfaceData.hpp"
#include "Vec2Data.hpp"

FillNode::FillNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "FillSurface");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
  _defaultGradient = QLinearGradient();
  _defaultGradient.setColorAt(0, "red");
}

unsigned int FillNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 3;
  default:
    return 1;
  };
}

NodeDataType FillNode::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return GradientData().type();
    default:
      return Vec2Data().type();
    }
  default:
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> FillNode::outData(PortIndex _portIndex) { return _content; }

void FillNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    switch (portIndex) {
    case 0:
      _gradient.reset();
      emit gradientChanged();
      break;
    case 1:
      _start.reset();
      emit startChanged();
      break;
    default:
      _end.reset();
      emit endChanged();
      break;
    }
    emit dataInvalidated(0);
  } else {
    switch (portIndex) {
    case 0:
      _gradient = data;
      emit gradientChanged();
      break;
    case 1:
      _start = data;
      emit startChanged();
      break;
    default:
      _end = data;
      emit endChanged();
      break;
    }
    emit dataUpdated(0);
  }
}

QString FillNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return "gradient";
    case 1:
      return "start";
    default:
      return "end";
    }
  default:
    return "out";
  }
}

QList<QVariant> FillNode::gradient() {
  auto gradient = _gradient.expired() ? _defaultGradient : _gradient.lock()->repr<QGradient>();
  QList<QVariant> newList;

  for (auto &stop : gradient.stops()) {
    QList<QVariant> newStop;
    newStop.append(QVariant::fromValue(stop.first));
    newStop.append(QVariant::fromValue(stop.second));
    newList.append(QVariant::fromValue(newStop));
  }

  return newList;
}
