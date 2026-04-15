#pragma once

#include <QDebug>
#include <QQuickItem>
#include <qdebug.h>
#include <qobject.h>
#include <qqmlengine.h>

class Receiver : public QObject { // NOLINT
  Q_OBJECT
  QML_ELEMENT

  public:
  std::unordered_multimap<std::string, int> driver_mapping;

  Receiver(QObject *parent = nullptr);
  ~Receiver() override;
};
