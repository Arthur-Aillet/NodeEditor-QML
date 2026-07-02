// #include "RegisteryAccess.hpp"
// #include "NodeDelegateModel.hpp"
// #include <memory>
// #include <qnamespace.h>
// #include <qvariant.h>

// RegisteryAccess::RegisteryAccess(QObject *parent,
//                                  std::shared_ptr<NodeDelegateModelRegistry> registry)
//     : QObject(parent), _registry(registry),
//     _nodeMapModel(std::make_shared<QStandardItemModel>()),
//       _filter("") {}

// void RegisteryAccess::setFilter(QString filter) {
//   if (filter == _filter) {
//     return;
//   }

//   _filter = filter;
//   emit filterChanged();
//   emit nodeMapModelChanged();
// }

// QString RegisteryAccess::getFilter() { return _filter; }

// QStandardItemModel *RegisteryAccess::getNodeMapModel() {
//   _nodeMapModel->clear();
//   QStandardItem *root = _nodeMapModel->invisibleRootItem();

//   const auto &categoryMap = _registry->registeredModelsCategoryAssociation();
//   const auto &modelInfosMap = _registry->registeredModelsInfosAssociation();

//   auto categoryList = QStringList(_registry->categories().begin(),
//   _registry->categories().end()); categoryList.sort(Qt::CaseInsensitive); for (const auto
//   &category : categoryList) {

//     QStringList modelsInCategory;

//     for (auto it = categoryMap.begin(); it != categoryMap.end(); ++it) {
//       const auto &[modelName, nodeCategory] = *it;
//       if (nodeCategory == category && modelName.contains(_filter, Qt::CaseInsensitive)) {
//         modelsInCategory.push_back(modelName);
//       }
//     }
//     modelsInCategory.sort(Qt::CaseInsensitive);

//     if (!modelsInCategory.empty()) {
//       QStandardItem *categoryItem = new QStandardItem(category);

//       for (const auto &modelName : modelsInCategory) {
//         QStandardItem *modelItem = new QStandardItem(modelName);
//         const NodeDelegateModel::ModelInfos &modelInfos = modelInfosMap.at(modelName);

//         modelItem->setData(QVariant::fromValue(modelInfos.ports), Qt::WhatsThisRole);
//         categoryItem->appendRow(modelItem);
//       }
//       root->appendRow(categoryItem);
//     }
//   }
//   return _nodeMapModel.get();
// }