#pragma once

#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <memory>
#include <qabstractitemmodel.h>
#include <qcolor.h>
#include <qlogging.h>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qquickitem.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

class ATypeCharacterNodeModel : public NodeDelegateModel {
  Q_OBJECT
  QML_INTERFACE

  public:
  Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)
  Q_PROPERTY(bool baseColorEditable READ baseColorEditable NOTIFY baseColorEditableChanged)
  Q_PROPERTY(double fontSize MEMBER _fontSize NOTIFY fontSizeChanged)

  Q_PROPERTY(double animationOpacitySpeed MEMBER _animationOpacitySpeed NOTIFY
                 animationOpacitySpeedChanged)
  Q_PROPERTY(
      double animationWidthSpeed MEMBER _animationWidthSpeed NOTIFY animationWidthSpeedChanged)
  Q_PROPERTY(double animationTransformSpeed MEMBER _animationTransformSpeed NOTIFY
                 animationTransformSpeedChanged)

  virtual QQmlComponent *getComponent() = 0;
  virtual QVariantMap getAdditionalProperties() = 0;
  ATypeCharacterNodeModel(QQmlEngine *engine) : NodeDelegateModel(engine) {}
  ~ATypeCharacterNodeModel() = default;

  Q_INVOKABLE void createCharacterObject(QQuickItem *instance, QString initialChar, int index);
  Q_INVOKABLE void destroyItem(QQuickItem *instance, int index);
  Q_INVOKABLE void setChar(QQuickItem *instance, int index, QString character);
  Q_INVOKABLE QString getString(QQuickItem *instance);

  QColor baseColor() {
    if (baseColorEditable()) {
      return _baseColor;
    } else {
      return _baseColorPtr.lock()->get<QColor>();
    }
  }

  void setBaseColor(QColor color) {
    if (_baseColor == color)
      return;
    _baseColor = color;
    emit baseColorChanged();
  }

  bool baseColorEditable() { return _baseColorPtr.expired(); }

  protected:
  QVector<QSharedPointer<QQuickItem>> _characters{};
  QColor _baseColor = "white";
  std::weak_ptr<NodeData> _baseColorPtr;
  double _fontSize = 150;
  double _animationOpacitySpeed = 250;
  double _animationWidthSpeed = 100;
  double _animationTransformSpeed = 250;

  public:
  signals:
  void baseColorChanged();
  void baseColorEditableChanged();
  void fontSizeChanged();
  void animationOpacitySpeedChanged();
  void animationWidthSpeedChanged();
  void animationTransformSpeedChanged();
};

Q_DECLARE_INTERFACE(ATypeCharacterNodeModel, "ATypeCharacterNodeModel")
