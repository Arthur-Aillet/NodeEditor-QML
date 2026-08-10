#include "NodeDelegateModel.hpp"

#include "Definitions.hpp"
#include "NodeStyle.hpp"
#include "StyleCollection.hpp"

NodeDelegateModel::NodeDelegateModel(QQmlEngine *engine)
    : _nodeStyle(StyleCollection::getNodeStyle()) {}

QJsonObject NodeDelegateModel::save() const { return QJsonObject(); }

NodeDelegateModel::ModelInfos NodeDelegateModel::modelInfos() const {
  QList<PortInfo> ports;

  auto collectPorts = [&](PortType portType) {
    for (unsigned int index = 0; index != nPorts(portType); index++) {
      ports.push_back({.caption = portCaption(portType, index),
                       .captionVisible = portCaptionVisible(portType, index),
                       .dataType = dataType(portType, index),
                       .portType = portType,
                       .portIndex = index});
    }
  };

  collectPorts(PortType::In);
  collectPorts(PortType::Out);

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

ConnectionPolicy NodeDelegateModel::portConnectionPolicy(PortType portType, PortIndex) const {
  switch (portType) {
  case PortType::In:
    return ConnectionPolicy::Replace;
  case PortType::Out:
  default:
    return ConnectionPolicy::Many;
  }
}

NodeStyle const &NodeDelegateModel::nodeStyle() const { return _nodeStyle; }

void NodeDelegateModel::setNodeStyle(NodeStyle const &style) { _nodeStyle = style; }

QPixmap NodeDelegateModel::processingStatusIcon() const {
  int resolution = _nodeStyle.processingIconStyle._resolution;
  switch (_processingStatus) {
  case NodeProcessingStatus::NoStatus:
    return {};
  case NodeProcessingStatus::Updated:
    return _nodeStyle.statusUpdated.pixmap(resolution);
  case NodeProcessingStatus::Processing:
    return _nodeStyle.statusProcessing.pixmap(resolution);
  case NodeProcessingStatus::Pending:
    return _nodeStyle.statusPending.pixmap(resolution);
  case NodeProcessingStatus::Empty:
    return _nodeStyle.statusEmpty.pixmap(resolution);
  case NodeProcessingStatus::Failed:
    return _nodeStyle.statusInvalid.pixmap(resolution);
  case NodeProcessingStatus::Partial:
    return _nodeStyle.statusPartial.pixmap(resolution);
  }

  return {};
}

void NodeDelegateModel::setStatusIcon(NodeProcessingStatus status, const QPixmap &pixmap) {
  switch (status) {
  case NodeProcessingStatus::NoStatus:
    break;
  case NodeProcessingStatus::Updated:
    _nodeStyle.statusUpdated = QIcon(pixmap);
    break;
  case NodeProcessingStatus::Processing:
    _nodeStyle.statusProcessing = QIcon(pixmap);
    break;
  case NodeProcessingStatus::Pending:
    _nodeStyle.statusPending = QIcon(pixmap);
    break;
  case NodeProcessingStatus::Empty:
    _nodeStyle.statusEmpty = QIcon(pixmap);
    break;
  case NodeProcessingStatus::Failed:
    _nodeStyle.statusInvalid = QIcon(pixmap);
    break;
  case NodeProcessingStatus::Partial:
    _nodeStyle.statusPartial = QIcon(pixmap);
    break;
  }
}

void NodeDelegateModel::setStatusIconStyle(const ProcessingIconStyle &style) {
  _nodeStyle.processingIconStyle = style;
}

void NodeDelegateModel::setNodeProcessingStatus(NodeProcessingStatus status) {
  _processingStatus = status;
}

void NodeDelegateModel::setBackgroundColor(QColor const &color) {
  _nodeStyle.setBackgroundColor(color);
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