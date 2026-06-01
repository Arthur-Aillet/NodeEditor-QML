#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"
#include "TextData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class ATypeNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("")

  Q_PROPERTY(QString text READ getText NOTIFY textChanged)

  public:
  ATypeNode(QQmlEngine *engine);

  ~ATypeNode() = default;

  public:
  QString caption() const override { return QStringLiteral("AType font display"); }

  bool captionVisible() const override { return true; }

  QString name() const override { return QStringLiteral("AType"); }

  public:
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  std::shared_ptr<SurfaceData> createATypeSurfaceData(QQmlEngine *engine);

  QString getText() {
    if (_text == nullptr) {
      return "";
    }
    return _text->text;
  }

  signals:
  void valueUpdated(QQmlComponent *component);
  void textChanged();

  private:
  bool _connected = false;

  std::shared_ptr<TextData> _text = nullptr;
  std::shared_ptr<SurfaceData> _content;
};
