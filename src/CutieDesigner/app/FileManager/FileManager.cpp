#include "FileManager.hpp"
#include <qjsonobject.h>

FileManager::FileManager(std::shared_ptr<DataFlowGraphModel> model, QObject *parent)
    : QObject(parent), _model(model) {}

void FileManager::save() {
  qDebug() << "save";
  QJsonObject jsonModel = _model->save();
  qDebug() << jsonModel;
}

void FileManager::load() {
  qDebug() << "load";
  qDebug() << "load";
}
