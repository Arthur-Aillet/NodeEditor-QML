#pragma once

#include <QObject>
#include <QtQmlIntegration>
#include <qqmlintegration.h>

class Clipboard : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  Q_PROPERTY(QJsonObject content READ content WRITE setContent)

  Clipboard(QObject *parent = nullptr);

  QJsonObject content();
  void setContent(QJsonObject text);
};
