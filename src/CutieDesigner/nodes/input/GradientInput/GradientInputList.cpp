#include "GradientInputList.hpp"

int GradientInputList::rowCount(const QModelIndex &parent) const { return _colors.size(); }

QVariant GradientInputList::data(const QModelIndex &index, int _role) const {
  return _colors.at(index.row());
}

void GradientInputList::setPort(bool val, int portIndex) { _colors[portIndex] = val; }

void GradientInputList::pushPort() {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  _colors.push_back(false);
  endRemoveRows();
}

void GradientInputList::popPort() {
  beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
  _colors.pop_back();
  endRemoveRows();
}
