#include "MyNodeEditor.hpp"


std::shared_ptr<QtNodes::NodeDelegateModelRegistry> MyNodeEditor::registerDataModels() const
{
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

    return ret;
}