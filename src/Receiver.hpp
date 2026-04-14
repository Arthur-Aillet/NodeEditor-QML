#pragma once

#include <QDebug>
#include <QQuickItem>
#include <qdebug.h>
#include <qobject.h>
#include <qqmlengine.h>

class Receiver : public QObject { // NOLINT
  Q_OBJECT

  public:
  std::unordered_multimap<std::string, QMetaProperty> driver_mapping;

  Receiver(QObject *parent = nullptr);
  ~Receiver() override;
};
