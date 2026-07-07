#pragma once

#include "DataFlowGraphModel.hpp"
#include "ModelInterface.hpp"
#include <memory>

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  std::shared_ptr<DataFlowGraphModel> graphModel;

  Q_PROPERTY(DataFlowGraphModel *dataFlowGraph READ getDataFlowGraphModel CONSTANT);

  protected:
  DataFlowGraphModel *getDataFlowGraphModel() { return graphModel.get(); }

  public:
  static DataFlowModelInterface *create(QQmlEngine *_qmlEngine, QJSEngine *_jsEngine);
  static DataFlowModelInterface *init(std::shared_ptr<DataFlowGraphModel> _graphModel);

  signals:
  void registeryChanged();

  protected:
  DataFlowModelInterface(std::shared_ptr<DataFlowGraphModel> _graphModel);
};
