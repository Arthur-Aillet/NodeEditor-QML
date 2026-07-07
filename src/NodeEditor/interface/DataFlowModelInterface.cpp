#include "DataFlowModelInterface.hpp"
#include <memory>

DataFlowModelInterface *DataFlowModelInterface::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "Data Flow Model Interface never setup.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return dynamic_cast<DataFlowModelInterface *>(instance);
}

DataFlowModelInterface *
DataFlowModelInterface::init(std::shared_ptr<DataFlowGraphModel> _graphModel) {
  instance = new DataFlowModelInterface(_graphModel);
  return dynamic_cast<DataFlowModelInterface *>(instance);
};

DataFlowModelInterface::DataFlowModelInterface(std::shared_ptr<DataFlowGraphModel> _graphModel)
    : ModelInterface(_graphModel), graphModel(_graphModel) {}