#pragma once

#include "ModelInterface.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include "RegisteryAccess.hpp"

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::DataFlowGraphModel &graphModel;
  RegisteryAccess registeryAccess;

  Q_PROPERTY(RegisteryAccess registery MEMBER registeryAccess NOTIFY registeryChanged)

  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static DataFlowModelInterface *init(QtNodes::DataFlowGraphModel &_graphModel);

  signals:
  void registeryChanged();

  protected:
  DataFlowModelInterface(QtNodes::DataFlowGraphModel &_graphModel);
};
