#pragma once

#include "NodeDelegateModelRegistry.hpp"
#include <memory>
#include <qqmlintegration.h>
#include <qstandarditemmodel.h>
#include <qtmetamacros.h>

class RegisteryAccess {
  Q_GADGET
  QML_VALUE_TYPE(registeryAccess)

  std::shared_ptr<NodeDelegateModelRegistry> registry = nullptr;

  public:
  Q_PROPERTY(QStandardItemModel *nodeMapModel MEMBER _nodeMapModel NOTIFY nodeMapModelChanged)

  Q_INVOKABLE QStandardItemModel *filterNodeMapModel(QString filter);

  QStandardItemModel *_nodeMapModel = nullptr;

  RegisteryAccess();
  RegisteryAccess(std::shared_ptr<NodeDelegateModelRegistry> _reg);

  bool operator==(const RegisteryAccess &other) { return registry == other.registry; }

  signals:
  void nodeMapModelChanged();
};