#pragma once

#include <qabstractitemmodel.h>
#include <qqmlintegration.h>

class GradientInputList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int portsCount() const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  void setPort(bool surface, int nodeIndex);
  void pushPort();
  void popPort();

  protected:
  std::vector<bool> _colors = {{}};
};
