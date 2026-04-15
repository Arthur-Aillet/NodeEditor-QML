#pragma once

#include <QObject>
#include <qobject.h>
#include <qqml.h>
#include <qtmetamacros.h>

#include "Node.hpp"

class ColorSceneReceiver : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged);

  public:
  int m_value = 0;

  ColorSceneReceiver(QObject *parent = nullptr) : QObject(parent) {}

  signals:
  void valueChanged();
};
