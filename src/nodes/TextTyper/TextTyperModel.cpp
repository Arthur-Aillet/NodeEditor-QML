#include "TextTyperModel.hpp"
#include "DecimalData.hpp"
#include "TextData.hpp"

QString TextTyperModel::portCaption(PortType portType, PortIndex index) const {
  switch (portType) {
  case NodeEditor::PortType::In:
    if (index == 0)
      return QString("Type delay");
    if (index == 1)
      return QString("Speed scale");
  case NodeEditor::PortType::Out:
    return QString("Text");
  default:
    return QString();
  }
}

unsigned int TextTyperModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &TextTyperModel::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return TextData().type();
  }
}

std::shared_ptr<NodeData> TextTyperModel::outData(PortIndex port) { return _content; };

void TextTyperModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  qDebug() << data.get();
};

QQmlComponent TextTyperModel::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesignerModule", "TextTyper");
}

QVariantMap TextTyperModel::componentInitialProperties() {
  QVariantMap map;
  map["textTyper"] = QVariant::fromValue(this);
  return map;
}

void TextTyperModel::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _textTyperQml = loaded;
}