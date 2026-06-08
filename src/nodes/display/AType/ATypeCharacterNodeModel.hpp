#pragma once

#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <qabstractitemmodel.h>
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
  Q_PROPERTY(QColor baseColor MEMBER _baseColor NOTIFY baseColorChanged)
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

  Q_INVOKABLE void setContainer(QQuickItem *container) { _container = container; }
  Q_INVOKABLE void createCharacterObject(QString initialChar, int index);
  Q_INVOKABLE void destroyItem(int index);
  Q_INVOKABLE void setChar(int index, QString character);
  Q_INVOKABLE QString getString();

  protected:
  QQuickItem *_container;
  QVector<QSharedPointer<QQuickItem>> _characters{};
  QColor _baseColor = "white";
  double _fontSize = 150;
  double _animationOpacitySpeed = 250;
  double _animationWidthSpeed = 100;
  double _animationTransformSpeed = 250;

  public:
  signals:
  void baseColorChanged();
  void fontSizeChanged();
  void animationOpacitySpeedChanged();
  void animationWidthSpeedChanged();
  void animationTransformSpeedChanged();
};

Q_DECLARE_INTERFACE(ATypeCharacterNodeModel, "ATypeCharacterNodeModel")
