#pragma once

#include "DataFlowGraphModel.hpp"
#include "NodeEditorContext.hpp"

class DataFlowContext : public NodeEditorContext {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(DataFlowGraphModel *dataFlowGraphModel READ dataFlowGraphModel CONSTANT)

  DataFlowContext(DataFlowGraphModel *graphModel, QQmlEngine *engine)
      : _dataFlowGraphModel(graphModel), NodeEditorContext(graphModel, engine) {};

  private:
  DataFlowGraphModel *dataFlowGraphModel() { return _dataFlowGraphModel; }

  DataFlowGraphModel *_dataFlowGraphModel;
};