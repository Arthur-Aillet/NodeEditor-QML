#pragma once

#include "AbstractGraphModel.hpp"
#include "Definitions.hpp"
#include "QmlUndoCommands.hpp"
#include <QObject>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  AbstractGraphModel &graphModel;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(AbstractGraphModel &_graphModel);

  Q_PROPERTY(AbstractGraphModel *graph READ getGraphModel CONSTANT);

  protected:
  AbstractGraphModel *getGraphModel() { return &graphModel; }

  inline static ModelInterface *instance = nullptr;
  ModelInterface(AbstractGraphModel &_graphModel);
  QUndoStack undoStack;

  public:
  Q_INVOKABLE void createConnection(ConnectionId const id);
  Q_INVOKABLE void deleteConnection(ConnectionId const id);
  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos);
};
