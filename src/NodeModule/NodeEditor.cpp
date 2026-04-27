#include "NodeEditor.hpp"
#include "NodeModel.hpp"
#include "ValueNodeModel.hpp"

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> NodeEditor::registerDataModels() const {
  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionNode>("Process");
  return ret;
}