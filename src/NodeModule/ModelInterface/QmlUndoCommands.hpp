#pragma once

#include "QtNodes/internal/Definitions.hpp"
#include <QUndoCommand>
#include <QtCore/QJsonObject>
#include <QtCore/QPointF>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class ModelInterface;

class CreateCommand : public QUndoCommand {
  public:
  CreateCommand(ModelInterface *interface, QString const name, QPointF const &mouseScenePos);

  void undo() override;
  void redo() override;

  private:
  ModelInterface *_interface;
  NodeId _nodeId;
  QJsonObject _sceneJson;
};

class ConnectCommand : public QUndoCommand {
  public:
  ConnectCommand(ModelInterface *scene, ConnectionId const);

  void undo() override;
  void redo() override;

  private:
  ModelInterface *_scene;

  ConnectionId _connId;
};

/**
 * Selected scene objects are serialized and then removed from the scene.
 * The deleted elements could be restored in `undo`.
 */
/*
class NODE_EDITOR_PUBLIC DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(ModelInterface *scene);

    void undo() override;
    void redo() override;

private:
    ModelInterface *_scene;
    QJsonObject _sceneJson;
};

class NODE_EDITOR_PUBLIC CopyCommand : public QUndoCommand
{
public:
    CopyCommand(ModelInterface *scene);
};

class NODE_EDITOR_PUBLIC PasteCommand : public QUndoCommand
{
public:
    PasteCommand(ModelInterface *scene, QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QJsonObject takeSceneJsonFromClipboard();
    QJsonObject makeNewNodeIdsInScene(QJsonObject const &sceneJson);

private:
    ModelInterface *_scene;
    QPointF const &_mouseScenePos;
    QJsonObject _newSceneJson;
};

class NODE_EDITOR_PUBLIC DisconnectCommand : public QUndoCommand
{
public:
    DisconnectCommand(ModelInterface *scene, ConnectionId const);

    void undo() override;
    void redo() override;

private:
    ModelInterface *_scene;

    ConnectionId _connId;
};

class NODE_EDITOR_PUBLIC MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(ModelInterface *scene, QPointF const &diff);

    void undo() override;
    void redo() override;
*/

/**
 * A command ID is used in command compression. It must be an integer unique to
 * this command's class, or -1 if the command doesn't support compression.
 */
// int id() const override;

/**
 * Several sequential movements could be merged into one command.
 *//*
    bool mergeWith(QUndoCommand const *c) override;

private:
    ModelInterface *_scene;
    std::unordered_set<NodeId> _selectedNodes;
    QPointF _diff;
};*/