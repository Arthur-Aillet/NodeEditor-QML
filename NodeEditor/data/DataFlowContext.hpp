#pragma once

#include "AbstractContext.hpp"
#include "DataFlowGraphModel.hpp"

class DataFlowContext : public AbstractContext {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(DataFlowGraphModel *dataFlowGraphModel READ dataFlowGraphModel CONSTANT)

  DataFlowContext(DataFlowGraphModel *graphModel)
      : _dataFlowGraphModel(graphModel),
        AbstractContext(graphModel, graphModel->registry()->engine()) {};
  ~DataFlowContext() = default;

  protected:
  DataFlowGraphModel *dataFlowGraphModel() { return _dataFlowGraphModel; }

  DataFlowGraphModel *_dataFlowGraphModel;
};