#include "NodeEditor.hpp"

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> NodeEditor::registerDataModels() const {
  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  return ret;
}