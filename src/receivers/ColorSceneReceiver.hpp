#pragma once

#include <QObject>
#include <qobject.h>
#include <qqml.h>
#include <qtmetamacros.h>

#include "Receiver.hpp"

class ColorSceneReceiver : public Receiver {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged);

  public:
  int m_value = 0;

  ColorSceneReceiver(QObject *parent = nullptr) : Receiver(parent) {
    auto metaObject = ColorSceneReceiver::staticMetaObject;

    for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); ++i) {
      driver_mapping.insert(std::pair("intensity", metaObject.property(i)));
    }
  }

  signals:
  void valueChanged();
};
