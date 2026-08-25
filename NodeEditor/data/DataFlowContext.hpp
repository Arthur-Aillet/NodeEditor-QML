#pragma once

#include "AbstractContext.hpp"
#include "DataFlowGraphModel.hpp"

class DataFlowContext : public AbstractContext {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(DataFlowGraphModel *dataFlowGraphModel READ dataFlowGraphModel CONSTANT)

  DataFlowContext(DataFlowGraphModel *graphModel, QQmlEngine *engine)
      : _dataFlowGraphModel(graphModel), AbstractContext(graphModel, engine) {};

  protected:
  DataFlowGraphModel *dataFlowGraphModel() { return _dataFlowGraphModel; }

  DataFlowGraphModel *_dataFlowGraphModel;
};