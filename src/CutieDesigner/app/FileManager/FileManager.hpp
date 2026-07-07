#pragma once

#include "DataFlowGraphModel.hpp"

#include <QMetaMethod>
#include <QMetaObject>
#include <QQmlContext>
#include <QtQmlIntegration>

#include <memory>

class FileManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("C++ managed")

  public:
  FileManager(std::shared_ptr<DataFlowGraphModel> model, QObject *parent = nullptr);

  Q_INVOKABLE bool graphEmpty();
  Q_INVOKABLE QString saveGraph(QUrl path);
  Q_INVOKABLE QString loadGraph(QUrl path, bool overwrite);

  private:
  std::shared_ptr<DataFlowGraphModel> _model;
};