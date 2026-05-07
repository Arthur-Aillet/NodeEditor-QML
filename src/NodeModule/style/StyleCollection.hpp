#pragma once

#include "QmlConnectionStyle.hpp"
#include "QmlGraphicsViewStyle.hpp"
#include <QObject>
#include <qqmlintegration.h>

class StyleCollection : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  StyleCollection(QObject *parent = nullptr) : QObject(parent) {}

  Q_PROPERTY(QmlConnectionStyle connectionStyle MEMBER ConnectionStyle);
  Q_PROPERTY(QmlGraphicsViewStyle graphicsViewStyle MEMBER GraphicsViewStyle);

  QmlConnectionStyle ConnectionStyle;
  QmlGraphicsViewStyle GraphicsViewStyle;
};
