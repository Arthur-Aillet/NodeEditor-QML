#pragma once

#include <QObject>
#include <QtQmlIntegration>
#include <qqmlintegration.h>

class Clipboard : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  Q_PROPERTY(QString text READ text WRITE setText CONSTANT)

  Clipboard(QObject *parent = nullptr);

  QString text();
  void setText(QString text);
};