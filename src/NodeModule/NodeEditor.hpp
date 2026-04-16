#pragma once

#include <QtNodes/QmlWrapper>
#include <qqmlintegration.h>

class NodeEditor : public QtNodes::QmlWrapper {
  Q_OBJECT
  QML_ELEMENT

  public:
  explicit NodeEditor(QQuickItem *parent = nullptr) : QtNodes::QmlWrapper(parent) {}

  protected:
  [[nodiscard]] std::shared_ptr<QtNodes::NodeDelegateModelRegistry>
  registerDataModels() const override;
};