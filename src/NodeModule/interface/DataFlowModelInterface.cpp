#include "DataFlowModelInterface.hpp"

DataFlowModelInterface *DataFlowModelInterface::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "Data Flow Model Interface never setup.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return dynamic_cast<DataFlowModelInterface *>(instance);
}

DataFlowModelInterface *DataFlowModelInterface::init(DataFlowGraphModel &_graphModel) {
  instance = new DataFlowModelInterface(_graphModel);
  return dynamic_cast<DataFlowModelInterface *>(instance);
};

DataFlowModelInterface::DataFlowModelInterface(DataFlowGraphModel &_graphModel)
    : ModelInterface(_graphModel), graphModel(_graphModel),
      registeryAccess(graphModel.dataModelRegistry()) {}

void RegisteryAccess::nodeMapModelChanged() {}