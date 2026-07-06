#pragma once

#include "Definitions.hpp"
#include "NodeData.hpp"
#include "NodeStyle.hpp"
#include "Serializable.hpp"

#include <QJSEngine>
#include <QList>
#include <QQmlComponent>
#include <QQuickItem>
#include <QtGui/QColor>
#include <qqmlintegration.h>

/**
 * Describes whether a node configuration is usable and defines a description message
 */
struct NodeValidationState {
  enum class State : int {
    Valid = 0,   ///< All required inputs are present and correct.
    Warning = 1, ///< Some inputs are missing or questionable, processing may be unreliable.
    Error = 2,   ///< Inputs or settings are invalid, preventing successful computation.
  };
  bool isValid() { return _state == State::Valid; };
  QString const message() { return _stateMessage; }
  State state() { return _state; }

  State _state{State::Valid};
  QString _stateMessage{""};
};

/**
 * Describes the node status, depending on its current situation
 */
enum class NodeProcessingStatus : int {
  NoStatus = 0,   ///< No processing status is shown in the Node UI.
  Updated = 1,    ///< Node is up to date; its outputs reflect the current inputs and parameters.
  Processing = 2, ///< Node is currently running a computation.
  Pending = 3,    ///< Node is out of date and waiting to be recomputed (e.g. manual/queued run).
  Empty = 4,      ///< Node has no valid input data; nothing to compute.
  Failed = 5,     ///< The last computation ended with an error.
  Partial = 6,    ///< Computation finished incompletely; only partial results are available.
};

class StyleCollection;

struct PortInfo {
  Q_GADGET
  QML_VALUE_TYPE(portInfo)
  QML_STRUCTURED_VALUE

  public:
  QString caption;
  bool captionVisible;
  NodeDataType dataType;
  PortType portType;
  PortIndex portIndex;

  Q_PROPERTY(QString caption MEMBER caption)
  Q_PROPERTY(bool captionVisible MEMBER captionVisible)
  Q_PROPERTY(NodeDataType dataType MEMBER dataType)
  Q_PROPERTY(PortType portType MEMBER portType)
  Q_PROPERTY(PortIndex portIndex MEMBER portIndex)
};

/**
 * The class wraps Node-specific data operations and propagates it to
 * the nesting DataFlowGraphModel which is a subclass of
 * AbstractGraphModel.
 * This class is the same what has been called NodeDataModel before v3.
 */
class NodeDelegateModel : public QObject, public Serializable {
  Q_OBJECT

  public:
  struct ModelInfos {
    QString name;
    QList<PortInfo> ports;
  };

  NodeDelegateModel(QQmlEngine *engine);

  virtual ~NodeDelegateModel() = default;

  /// Get inital model infos
  ModelInfos modelInfos() const;

  /// Name makes this model unique
  virtual QString name() const = 0;

  /// Caption is used in GUI (defaults to name)
  virtual QString caption() const { return name(); };
  /// It is possible to hide caption in GUI
  virtual bool captionVisible() const { return true; }

  virtual unsigned int nPorts(PortType portType) const = 0;
  /// Port caption is used in GUI to label individual ports, empty by default
  virtual QString portCaption(PortType, PortIndex) const { return ""; }
  /// It is possible to hide port caption in GUI
  virtual bool portCaptionVisible(PortType, PortIndex) const { return false; }
  virtual ConnectionPolicy portConnectionPolicy(PortType, PortIndex) const;

  /// Nicknames can be assigned to nodes and shown in GUI, empty by default
  virtual QString label() const { return ""; }
  /// It is possible to hide the nickname in GUI
  virtual bool labelVisible() const { return false; }
  /// Controls whether the label can be edited or not
  virtual bool labelEditable() const { return false; }

  /// Validation State will default to Valid, but you can manipulate it by overriding in an
  /// inherited class
  virtual NodeValidationState validationState() const { return _nodeValidationState; }
  /// Returns the curent processing status
  virtual NodeProcessingStatus processingStatus() const { return _processingStatus; }

  /// Progress is used in GUI
  virtual QString progressValue() const { return _progressValue; }

  virtual NodeFlags flags() const { return _flags; }
  virtual void setFlags(NodeFlags flags) { _flags = flags; }

  QJsonObject save() const override;
  void load(QJsonObject const &) override;

  void setNodeProcessingStatus(NodeProcessingStatus status);
  void setValidationState(const NodeValidationState &validationState);

  virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

  NodeStyle const &nodeStyle() const;
  void setNodeStyle(NodeStyle const &style);

  /// Convenience helper to change the node background color.
  void setBackgroundColor(QColor const &color);

  QPixmap processingStatusIcon() const;
  void setStatusIcon(NodeProcessingStatus status, const QPixmap &pixmap);
  void setStatusIconStyle(ProcessingIconStyle const &style);
  void setProgressValue(QString new_progress) { _progressValue = new_progress; }

  virtual void setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex) = 0;
  virtual std::shared_ptr<NodeData> outData(PortIndex const port) = 0;

  void createComponent(QQuickItem *container, QQmlEngine *engine);
  virtual QQmlComponent embeddedComponent(QQmlEngine *engine) { return QQmlComponent(); };
  virtual QVariantMap componentInitialProperties() { return QVariantMap(); }

  public slots:
  virtual void inputConnectionCreated(ConnectionId const &) {}
  virtual void inputConnectionDeleted(ConnectionId const &) {}
  virtual void outputConnectionCreated(ConnectionId const &) {}
  virtual void outputConnectionDeleted(ConnectionId const &) {}
  virtual void embeddedComponentLoaded(std::shared_ptr<QQuickItem> instance) {}

  signals:
  /// Triggers the updates in the nodes downstream.
  void dataUpdated(PortIndex const index);

  /// Triggers the propagation of the empty data downstream.
  // Currently acts just like dataUpdated
  void dataInvalidated(PortIndex const index);

  /**
   * @brief Call this function before deleting the data associated with ports.
   * The function notifies the Graph Model and makes it remove and recompute the
   * affected connection addresses.
   */
  void portsAboutToBeDeleted(PortType const portType, PortIndex const first, PortIndex const last);

  /// Call this function when data and port moditications are finished.
  void portsDeleted(PortType const portType);

  /**
   * @brief Call this function before inserting the data associated with ports.
   * The function notifies the Graph Model and makes it recompute the affected
   * connection addresses.
   */
  void portsAboutToBeInserted(PortType const portType, PortIndex const first, PortIndex const last);

  /// Call this function when data and port moditications are finished.
  void portsInserted(PortType const portType);

  /// Call this function when you want to refresh the ports names.
  void portsNameChanged(PortType const portType);

  private:
  std::shared_ptr<QQuickItem> _embed{nullptr};
  NodeStyle _nodeStyle;
  NodeFlags _flags;

  NodeValidationState _nodeValidationState;
  NodeProcessingStatus _processingStatus{NodeProcessingStatus::NoStatus};
  QString _progressValue{QString()};
};

Q_DECLARE_METATYPE(NodeValidationState)
Q_DECLARE_METATYPE(NodeProcessingStatus)
