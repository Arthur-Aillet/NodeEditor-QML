#pragma once

#include "Definitions.hpp"

#include "AbstractGraphModel.hpp"
#include "ConnectionIdHash.hpp"
#include "Definitions.hpp"
#include "NodeDelegateModelRegistry.hpp"
#include "Serializable.hpp"

#include <QJsonObject>
#include <QString>
#include <QtQmlIntegration>

#include <unordered_map>

namespace NodeEditor {
class DataFlowGraphModel : public AbstractGraphModel, public Serializable {
  Q_OBJECT
  QML_IMPLEMENTS_INTERFACES(NodeEditor::AbstractGraphModel)
  QML_ANONYMOUS

  public:
  struct NodeGeometryData {
    QSize size;
    QPointF pos;
  };

  public:
  Q_PROPERTY(NodeEditor::NodeDelegateModelRegistry *registry READ registry CONSTANT)

  DataFlowGraphModel(NodeDelegateModelRegistry *registry);

  NodeDelegateModelRegistry *registry() { return _registry; }

  Q_INVOKABLE QSet<ConnectionId> allConnectionIds(NodeId const nodeId) const override;
  QSet<ConnectionId> connections(NodeId nodeId, PortSide portSide,
                                 PortIndex portIndex) const override;

  bool connectionExists(ConnectionId const connectionId) const override;
  bool connectionPossible(ConnectionId const connectionId) const override;
  void addConnection(ConnectionId const connectionId) override;
  bool deleteConnection(ConnectionId const connectionId) override;

  QSet<NodeId> allNodeIds() const override;
  NodeId addNode(QString const nodeType) override;
  bool nodeExists(NodeId const nodeId) const override;
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, NodeRole role) const override;
  bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) override;
  bool deleteNode(NodeId const nodeId) override;

  bool clear() override;

  QVariant portData(NodeId nodeId, PortSide portSide, PortIndex portIndex,
                    PortRole role) const override;
  bool setPortData(NodeId nodeId, PortSide portSide, PortIndex portIndex, QVariant const &value,
                   PortRole role = PortRole::Data) override;

  Q_INVOKABLE QJsonObject saveNode(NodeId const) const override;
  Q_INVOKABLE NodeId loadNode(QJsonObject const &nodeJson) override;

  // From Serializable
  Q_INVOKABLE QJsonObject save() const override;
  Q_INVOKABLE void load(QJsonObject const &json) override;

  Q_INVOKABLE void createEmbed(NodeId nodeId, QQuickItem *container) const override;

  /**
   * Fetches the NodeDelegateModel for the given `nodeId` and tries to cast the
   * stored pointer to the given type
   */
  template <typename NodeDelegateModelType>
  NodeDelegateModelType *delegateModel(NodeId const nodeId) {
    auto it = _models.find(nodeId);
    if (it == _models.end())
      return nullptr;

    auto model = dynamic_cast<NodeDelegateModelType *>(it->second.get());

    return model;
  }

  /// Loops do not make any sense in uni-direction data propagation
  bool loopsEnabled() const override { return false; }

  signals:
  void inPortDataWasSet(NodeId const, PortSide const, PortIndex const);

  private:
  NodeId newNodeId() override;
  void connectNode(NodeDelegateModel *model, NodeId nodeId);
  void sendConnectionCreation(ConnectionId const connectionId);
  void sendConnectionDeletion(ConnectionId const connectionId);

  private slots:
  /**
   * Fuction is called in three cases:
   *
   * - By underlying NodeDelegateModel when a node has new data to propagate.
   *   @see DataFlowGraphModel::addNode
   * - When a new connection is created.
   *   @see DataFlowGraphModel::addConnection
   * - When a node restored from JSON an needs to send data downstream.
   *   @see DataFlowGraphModel::loadNode
   */
  void onOutPortDataUpdated(NodeId const nodeId, PortIndex const portIndex);

  /// Function is called after detaching a connection.
  void propagateEmptyDataTo(NodeId const nodeId, PortIndex const portIndex);

  protected:
  NodeDelegateModelRegistry *_registry;
  std::unordered_map<NodeId, std::unique_ptr<NodeDelegateModel>> _models;
  QSet<ConnectionId> _connectivity;
  mutable std::unordered_map<NodeId, NodeGeometryData> _nodeGeometryData;
  std::unordered_map<NodeId, QString> _labels;
  std::unordered_map<NodeId, bool> _labelsVisible;
};
} // namespace NodeEditor
