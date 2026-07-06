#include "TimeInputNode.hpp"
#include "DecimalData.hpp"
#include "TimeController.hpp"

TimeInputNode::TimeInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _outContent{0, 0, 0, 0, 0},
      _outData{std::make_shared<DecimalData>(_outContent[0]),
               std::make_shared<DecimalData>(_outContent[1]),
               std::make_shared<DecimalData>(_outContent[2]),
               std::make_shared<DecimalData>(_outContent[3]),
               std::make_shared<DecimalData>(_outContent[4])} {
  auto timeController = TimeController::get();
  if (timeController) {
    QObject::connect(timeController, &TimeController::currentFrameChanged, this, [&]() {
      emit dataUpdated(0);
      emit dataUpdated(3);
    });
    QObject::connect(timeController, &TimeController::minFrameChanged, this, [&]() {
      emit dataUpdated(1);
      emit dataUpdated(3);
    });
    QObject::connect(timeController, &TimeController::maxFrameChanged, this, [&]() {
      emit dataUpdated(2);
      emit dataUpdated(3);
    });
    QObject::connect(timeController, &TimeController::currentTimeChanged, this,
                     [&]() { emit dataUpdated(4); });
  }
}

unsigned int TimeInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 5;
  default:
    return 0;
  }
}

NodeDataType TimeInputNode::dataType(PortType _portType, PortIndex portIndex) const {
  return DecimalData().type();
}

QString TimeInputNode::portCaption(PortType _portType, PortIndex portIndex) const {
  switch (portIndex) {
  case 0:
    return "Frame";
  case 1:
    return "Start";
  case 2:
    return "End";
  case 3:
    return "Progress %";
  case 4:
  default:
    return "Time (s)";
  };
}

std::shared_ptr<NodeData> TimeInputNode::outData(PortIndex portIndex) {
  auto timeController = TimeController::get();

  switch (portIndex) {
  case 0:
    _outContent[portIndex] = timeController->getCurrentFrame();
    break;
  case 1:
    _outContent[portIndex] = timeController->getMinFrame();
    break;
  case 2:
    _outContent[portIndex] = timeController->getMaxFrame();
    break;
  case 3:
    _outContent[portIndex] =
        (double)(timeController->getCurrentFrame() - timeController->getMinFrame()) /
        (timeController->getMaxFrame() - timeController->getMinFrame());
    break;
  case 4:
  default:
    _outContent[portIndex] = timeController->getCurrentTime();
    break;
  };
  return _outData[portIndex];
}