#include "GradientInputList.hpp"

int GradientInputList::rowCount(const QModelIndex &parent) const { return _gradientStops.size(); }

QVariant GradientInputList::data(const QModelIndex &index, int role) const {
  auto color = _gradientStops.at(index.row());
  switch (static_cast<GradientInputRole>(role)) {
  case GradientInputRole::Pos:
    return color.first;
  default:
  case GradientInputRole::Color:
    return color.second;
  }
}

void GradientInputList::pushColor() {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  if (_gradientStops.length() == 0) {
    _gradientStops.push_back(QGradientStop(0.0, QColor("red")));
  } else {
    _gradientStops.push_back(QGradientStop(1.0, QColor("red")));
  }
  endInsertRows();
  gradient.setStops(_gradientStops);
  emit gradientChanged();
}

void GradientInputList::popColor() {
  if (_gradientStops.length() <= 0)
    return;

  beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
  _gradientStops.pop_back();
  endRemoveRows();
  gradient.setStops(_gradientStops);
  emit gradientChanged();
}

void GradientInputList::editColor(QColor color, int index) {
  if (color == _gradientStops[index].second)
    return;
  _gradientStops[index].second = color;
  gradient.setStops(_gradientStops);
  emit gradientChanged();
}

void GradientInputList::editPos(double pos, int index) {
  if (pos == _gradientStops[index].first)
    return;
  _gradientStops[index].first = pos;
  gradient.setStops(_gradientStops);
  emit gradientChanged();
}
