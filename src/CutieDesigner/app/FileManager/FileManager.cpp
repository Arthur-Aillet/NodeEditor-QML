#include "FileManager.hpp"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonparseerror.h>

FileManager::FileManager(std::shared_ptr<DataFlowGraphModel> model, QObject *parent)
    : QObject(parent), _model(model) {}

bool FileManager::graphEmpty() { return _model->allNodeIds().isEmpty(); }

QString FileManager::saveGraph(QUrl path) {
  QFile file(path.toLocalFile());

  if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
    return "Couldn't open given file.";
  }
  file.resize(0);
  QJsonObject jsonModel = _model->save();
  QJsonDocument jsonDoc(jsonModel);
  QByteArray str = jsonDoc.toJson(QJsonDocument::Indented);
  file.write(str);
  return "";
}

QString FileManager::loadGraph(QUrl path, bool overwrite) {
  QFile file(path.toLocalFile());

  if (!file.exists()) {
    return "Given file doesn't exist.";
  }
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return "Couldn't open given file.";
  }
  QByteArray content = file.readAll();
  QJsonParseError parseError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(content, &parseError);
  if (jsonDoc.isNull()) {
    return "Failed to load: " + parseError.errorString();
  }

  if (overwrite) {
    _model->clear();
  }

  _model->load(jsonDoc.object());
  return "";
}
