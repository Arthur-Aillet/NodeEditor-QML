#pragma once

#include "DataFlowGraphModel.hpp"
#include "ModelInterface.hpp"
#include "RegisteryAccess.hpp"

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  DataFlowGraphModel &graphModel;
  RegisteryAccess registeryAccess;

  Q_PROPERTY(RegisteryAccess registery MEMBER registeryAccess NOTIFY registeryChanged)
  Q_PROPERTY(DataFlowGraphModel *dataFlowGraph READ getDataFlowGraphModel);

  protected:
  DataFlowGraphModel *getDataFlowGraphModel() { return &graphModel; }

  public:
  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static DataFlowModelInterface *init(DataFlowGraphModel &_graphModel);

  signals:
  void registeryChanged();

  protected:
  DataFlowModelInterface(DataFlowGraphModel &_graphModel);
};
