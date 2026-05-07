#include "RegisteryAccess.hpp"

RegisteryAccess::RegisteryAccess() {}

RegisteryAccess::RegisteryAccess(std::shared_ptr<NodeDelegateModelRegistry> _reg)
    : registry(_reg), m_nodeMapModel(new QStandardItemModel()) {
  filterNodeMapModel("");
}

QStandardItemModel *RegisteryAccess::filterNodeMapModel(QString filter) {
  m_nodeMapModel->clear();
  QStandardItem *root = m_nodeMapModel->invisibleRootItem();

  auto categoryList = QStringList(registry->categories().begin(), registry->categories().end());
  categoryList.sort(Qt::CaseInsensitive);
  for (const auto &category : categoryList) {
    const auto &map = registry->registeredModelsCategoryAssociation();
    QStringList row;

    for (auto it = map.begin(); it != map.end(); ++it) {
      if (it->second == category && it->first.contains(filter, Qt::CaseInsensitive)) {
        row.push_back(it->first);
      }
    }
    row.sort(Qt::CaseInsensitive);

    if (!row.empty()) {
      QStandardItem *categoryItem = new QStandardItem(category);

      for (auto val : row) {
        categoryItem->appendRow(new QStandardItem(val));
      }
      root->appendRow(categoryItem);
    }
  }
  return m_nodeMapModel;
}