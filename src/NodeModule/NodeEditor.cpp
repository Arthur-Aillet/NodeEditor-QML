#include "NodeEditor.hpp"
#include "NodeModel.hpp"
#include "ValueNodeModel.hpp"

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> NodeEditor::registerDataModels() const {
  auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

  ret->registerModel<ValueNodeModel>("Input1");
  ret->registerModel<ValueNodeModel1>("Input2");
  ret->registerModel<ValueNodeModel2>("Input3");
  ret->registerModel<ValueNodeModel3>("Input4");
  ret->registerModel<ValueNodeModel5>("Input5");
  ret->registerModel<ValueNodeModel4>("Input6");
  ret->registerModel<ValueNodeModel>("Input7");
  ret->registerModel<ValueNodeModel>("Input8");
  ret->registerModel<ValueNodeModel>("Input9");
  ret->registerModel<ValueNodeModel>("Input10");
  ret->registerModel<ValueNodeModel>("Input11");
  ret->registerModel<ValueNodeModel>("Input12");
  ret->registerModel<ValueNodeModel>("Input13");
  ret->registerModel<ValueNodeModel>("Input14");
  ret->registerModel<ValueNodeModel>("Input15");
  ret->registerModel<ValueNodeModel>("Input16");
  ret->registerModel<ValueNodeModel>("Input17");
  ret->registerModel<ValueNodeModel>("Input");
  ret->registerModel<AdditionNode>("Process");
  return ret;
}