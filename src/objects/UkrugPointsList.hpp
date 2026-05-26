#pragma once

#include "UkrugPointsListModel.hpp"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <qobject.h>

// struct UkrugPoint {
//   Q_GADGET
//   QML_VALUE_TYPE(ukrugPoint)
//   QML_STRUCTURED_VALUE

//   public:
//   float angle;
//   int layer;
// };

// static const UkrugPoint A[] = {{0, 1}, {0, 2}, {360.0 * 1 / 3, 2}, {360.0 * 2 / 3, 2}};
// static const UkrugPoint NotA[] = {{90, 2}, {-90, 2}};

class UkrugPointsList : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  UkrugPointsList() {}

  QList<UkrugPoint> _points;
};
