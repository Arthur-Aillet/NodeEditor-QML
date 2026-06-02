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
};

Q_DECLARE_INTERFACE(ATypeCharacterNodeModel, "ATypeCharacterNodeModel")
