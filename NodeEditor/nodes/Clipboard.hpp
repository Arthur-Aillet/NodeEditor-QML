#pragma once

#include <QObject>
#include <QtQmlIntegration>

namespace NodeEditor {
class Clipboard : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  Q_PROPERTY(QJsonObject content READ content WRITE setContent)

  Clipboard(QObject *parent = nullptr);

  QJsonObject content();
  void setContent(QJsonObject text);
};
}; // namespace NodeEditor
