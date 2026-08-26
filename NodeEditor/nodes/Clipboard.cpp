#include "Clipboard.hpp"

#include <QClipboard>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

using namespace NodeEditor;

Clipboard::Clipboard(QObject *parent) : QObject(parent) {}

QJsonObject Clipboard::content() {
  QString type = "text/node-graph";
  auto mime = QGuiApplication::clipboard()->mimeData(QClipboard::Clipboard);
  auto data = mime->data(type);
  QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
  if (jsonDoc.isNull()) {
    return QJsonObject();
  }
  return jsonDoc.object();
}

void Clipboard::setContent(QJsonObject json) {
  QString type = "text/node-graph";
  QJsonDocument jsonDoc(json);
  QByteArray data = jsonDoc.toJson(QJsonDocument::Compact);

  QMimeData *mimeData = new QMimeData;
  mimeData->setData(type, data);

  QGuiApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
}
