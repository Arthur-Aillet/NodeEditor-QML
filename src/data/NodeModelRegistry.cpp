#include "NodeModelRegistry.hpp"

using namespace NodeEditor;

std::unique_ptr<NodeModel> NodeModelRegistry::create(QString const &modelName) const {
  auto it = _registeredItemCreators.find(modelName);

  if (it != _registeredItemCreators.end()) {
    return it->second();
  }

  return nullptr;
}

NodeModelRegistry::RegisteredModelCreatorsMap const &
NodeModelRegistry::registeredModelCreators() const {
  return _registeredItemCreators;
}

NodeModelRegistry::Categories const &NodeModelRegistry::categories() const { return _categories; }
NodeModelRegistry::Categories NodeModelRegistry::categories() { return _categories; }
