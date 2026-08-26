#include "NodeDelegateModel.hpp"

#include "Definitions.hpp"
#include "NodeStyle.hpp"

#include <optional>

NodeDelegateModel::NodeDelegateModel(QQmlEngine *engine) {}

QJsonObject NodeDelegateModel::save() const { return QJsonObject(); }

NodeDelegateModel::ModelInfos NodeDelegateModel::modelInfos() const {
  QList<PortInfo> ports;

  auto collectPorts = [&](PortSide portSide) {
    for (unsigned int index = 0; index != nPorts(portSide); index++) {
      ports.push_back({.caption = portCaption(portSide, index),
                       .captionVisible = portCaptionVisible(portSide, index),
                       .dataType = dataType(portSide, index),
                       .portSide = portSide,
                       .portIndex = index});
    }
  };

  collectPorts(PortSide::In);
  collectPorts(PortSide::Out);

  return ModelInfos{
      .name = name(),
      .ports = ports,
  };
};

void NodeDelegateModel::load(QJsonObject const &) {
  //
}

void NodeDelegateModel::setValidationState(const NodeValidationState &validationState) {
  _nodeValidationState = validationState;
}

ConnectionPolicy NodeDelegateModel::portConnectionPolicy(PortSide portSide, PortIndex) const {
  switch (portSide) {
  case PortSide::In:
    return ConnectionPolicy::Replace;
  case PortSide::Out:
  default:
    return ConnectionPolicy::Many;
  }
}

std::optional<NodeStyle> const &NodeDelegateModel::nodeStyle() const { return _nodeStyle; }

void NodeDelegateModel::setNodeStyle(std::optional<NodeStyle> const &style) { _nodeStyle = style; }

QPixmap NodeDelegateModel::processingStatusIcon() const {
  // int resolution = _nodeStyle.processingIconStyle._resolution;
  // switch (_processingStatus) {
  // case NodeProcessingStatus::NoStatus:
  //   return {};
  // case NodeProcessingStatus::Updated:
  //   return _nodeStyle.statusUpdated.pixmap(resolution);
  // case NodeProcessingStatus::Processing:
  //   return _nodeStyle.statusProcessing.pixmap(resolution);
  // case NodeProcessingStatus::Pending:
  //   return _nodeStyle.statusPending.pixmap(resolution);
  // case NodeProcessingStatus::Empty:
  //   return _nodeStyle.statusEmpty.pixmap(resolution);
  // case NodeProcessingStatus::Failed:
  //   return _nodeStyle.statusInvalid.pixmap(resolution);
  // case NodeProcessingStatus::Partial:
  //   return _nodeStyle.statusPartial.pixmap(resolution);
  // }

  return {};
}

void NodeDelegateModel::setStatusIcon(NodeProcessingStatus status, const QPixmap &pixmap) {
  // switch (status) {
  // case NodeProcessingStatus::NoStatus:
  //   break;
  // case NodeProcessingStatus::Updated:
  //   _nodeStyle.statusUpdated = QIcon(pixmap);
  //   break;
  // case NodeProcessingStatus::Processing:
  //   _nodeStyle.statusProcessing = QIcon(pixmap);
  //   break;
  // case NodeProcessingStatus::Pending:
  //   _nodeStyle.statusPending = QIcon(pixmap);
  //   break;
  // case NodeProcessingStatus::Empty:
  //   _nodeStyle.statusEmpty = QIcon(pixmap);
  //   break;
  // case NodeProcessingStatus::Failed:
  //   _nodeStyle.statusInvalid = QIcon(pixmap);
  //   break;
  // case NodeProcessingStatus::Partial:
  //   _nodeStyle.statusPartial = QIcon(pixmap);
  //   break;
  // }
}

void NodeDelegateModel::setStatusIconStyle(const ProcessingIconStyle &style) {
  // _nodeStyle.processingIconStyle = style;
}

void NodeDelegateModel::setNodeProcessingStatus(NodeProcessingStatus status) {
  _processingStatus = status;
}

void NodeDelegateModel::createComponent(QQuickItem *container, QQmlEngine *engine) {
  QQmlComponent component = embeddedComponent(engine);
  if (component.isNull()) {
    return;
  }
  if (component.isError()) {
    qCritical() << "Embedded Node Componenent failed to load: \n" << component.errorString();
    return;
  }
  _embed = std::shared_ptr<QQuickItem>(qobject_cast<QQuickItem *>(
      component.createWithInitialProperties(componentInitialProperties())));
  _embed->setParentItem(container);
  QJSEngine::setObjectOwnership(_embed.get(), QJSEngine::CppOwnership);
  emit embeddedComponentLoaded(_embed);
};