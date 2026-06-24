#pragma once

#include "ATypeCharacterNodeModel.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qcontainerfwd.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlcontext.h>
#include <qqmlengine.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

class ATypeNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  Q_PROPERTY(QString text READ getText NOTIFY textChanged)
  Q_PROPERTY(ATypeCharacterNodeModel *character MEMBER _charModel NOTIFY charChanged)

  public:
  ATypeNode(QQmlEngine *engine);
  ~ATypeNode() = default;

  QString caption() const override { return QStringLiteral("AType font display"); }
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("AType"); }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
  std::shared_ptr<SurfaceData> createATypeSurfaceData(QQmlEngine *engine);

  QString getText() {
    if (_text.expired()) {
      return "";
    }
    return _text.lock()->repr<QString>();
  }

  signals:
  void valueUpdated(QQmlComponent *component);
  void textChanged();
  void charChanged();

  private:
  std::weak_ptr<NodeData> _text;
  ATypeCharacterNodeModel *_charModel = nullptr;
  std::shared_ptr<SurfaceData> _content;
};
