#pragma once

#include "DataFlowGraphModel.hpp"
#include "ModelInterface.hpp"

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  DataFlowGraphModel *graphModel;

  Q_PROPERTY(DataFlowGraphModel *dataFlowGraph READ getDataFlowGraphModel CONSTANT);

  protected:
  DataFlowGraphModel *getDataFlowGraphModel() { return graphModel; }

  public:
  static DataFlowModelInterface *create(QQmlEngine *_qmlEngine, QJSEngine *_jsEngine);
  static DataFlowModelInterface *init(DataFlowGraphModel *_graphModel);

  signals:
  void registeryChanged();

  protected:
  DataFlowModelInterface(DataFlowGraphModel *_graphModel);
};
