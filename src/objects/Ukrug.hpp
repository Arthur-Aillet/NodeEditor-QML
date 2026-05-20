#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

struct UkrugPoint {
  Q_GADGET
  QML_VALUE_TYPE(ukrugPoint)
  QML_STRUCTURED_VALUE

  public:
  float angle;
  int layer;
};

static const UkrugPoint A[] = {{0, 1}, {0, 2}, {360.0 * 1 / 3, 2}, {360.0 * 2 / 3, 2}};
static const UkrugPoint NotA[] = {{90, 2}, {-90, 2}};

class UkrugPointsListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  UkrugPointsListModel() {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const override { return _points.count(); }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
    if (!checkIndex(index))
      return QVariant();

    const auto &rowData = _points.at(index.row());
    switch (static_cast<PointRoles>(role)) {
    case PointRoles::Layer:
      return rowData.layer;
      break;
    case PointRoles::Angle:
      return rowData.angle;
      break;
    }

    return QVariant();
  }

  Q_INVOKABLE void print() {
    qDebug() << "\nPrinting from UkrugPointsListModel::print()";
    for (const auto &point : _points) {
      qDebug() << point.layer << ": " << point.angle << "°";
    }
  }

  Q_INVOKABLE void setLetter(QString letter);

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(PointRoles::Layer), "layer"},
            {static_cast<int>(PointRoles::Angle), "angle"}};
  }

  private:
  enum class PointRoles { Layer = Qt::UserRole + 1, Angle };

  QList<UkrugPoint> _points;
};
