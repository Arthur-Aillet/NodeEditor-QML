#include "FileManager.hpp"
#include "Definitions.hpp"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

FileManager::FileManager(std::shared_ptr<DataFlowGraphModel> model, QObject *parent)
    : QObject(parent), _model(model) {}

bool FileManager::graphEmpty() { return _model->allNodeIds().isEmpty(); }

static void removeStartNode(QJsonObject &json) {
  auto nodes = json["nodes"];
  QJsonArray keptNodes;
  for (const auto &node : nodes.toArray()) {
    if (!node.toObject()["flags"].toVariant().value<NodeFlags>().testFlag(NodeFlag::Locked)) {
      keptNodes.append(node);
    }
  }
  json["nodes"] = keptNodes;
}

QString FileManager::saveGraph(QUrl path) {
  QFile file(path.toLocalFile());

  if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
    return "Couldn't open given file.";
  }
  file.resize(0);
  QJsonObject jsonModel = _model->save();
  removeStartNode(jsonModel);
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
    for (auto &nodeId : _model->allNodeIds()) {
      if (!_model->nodeData(nodeId, NodeRole::Flags)
               .value<NodeFlags>()
               .testFlag(NodeFlag::Locked)) {
        _model->deleteNode(nodeId);
      }
    }
  }

  _model->load(jsonDoc.object());
  return "";
}
