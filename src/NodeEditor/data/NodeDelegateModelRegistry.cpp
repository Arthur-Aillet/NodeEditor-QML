#include "NodeDelegateModelRegistry.hpp"

std::unique_ptr<NodeDelegateModel> NodeDelegateModelRegistry::create(QString const &modelName) {
  auto it = _registeredItemCreators.find(modelName);

  if (it != _registeredItemCreators.end()) {
    return it->second();
  }

  return nullptr;
}

NodeDelegateModelRegistry::RegisteredModelCreatorsMap const &
NodeDelegateModelRegistry::registeredModelCreators() const {
  return _registeredItemCreators;
}

NodeDelegateModelRegistry::Categories const &NodeDelegateModelRegistry::categories() const {
  return _categories;
}
NodeDelegateModelRegistry::Categories NodeDelegateModelRegistry::categories() {
  return _categories;
}
