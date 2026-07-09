#include "Clipboard.hpp"

#include <QClipboard>
#include <QGuiApplication>
#include <QObject>

Clipboard::Clipboard(QObject *parent) : QObject(parent) {}

QString Clipboard::text() {
  QString type = "text/cute";
  auto mime = QGuiApplication::clipboard()->mimeData(QClipboard::Clipboard);
  auto data = mime->data(type);
  return QString::fromStdString(data.toStdString());
}

void Clipboard::setText(QString text) {
  QString type = "text/cute";
  QByteArray csvData(text.toStdString());

  QMimeData *mimeData = new QMimeData;
  mimeData->setData(type, csvData);

  QGuiApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
}
