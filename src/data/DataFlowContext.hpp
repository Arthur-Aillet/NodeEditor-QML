#pragma once

#include "AbstractContext.hpp"
#include "DataFlowGraph.hpp"

namespace NodeEditor {
class DataFlowContext : public AbstractContext {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provided from C++")

  public:
  Q_PROPERTY(NodeEditor::DataFlowGraph *dataFlowGraph READ dataFlowGraph CONSTANT)

  DataFlowContext(DataFlowGraph *graphModel)
      : _dataFlowGraph(graphModel),
        AbstractContext(graphModel, graphModel->registry()->engine()) {};
  ~DataFlowContext() = default;

  protected:
  DataFlowGraph *dataFlowGraph() { return _dataFlowGraph; }

  DataFlowGraph *_dataFlowGraph;
};
} // namespace NodeEditor
