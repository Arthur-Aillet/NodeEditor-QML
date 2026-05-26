#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include <qdebug.h>
#include <qdir.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qtmetamacros.h>

struct UkrugPoint {
  Q_GADGET
  QML_VALUE_TYPE(ukrugPoint)
  QML_STRUCTURED_VALUE

  public:
  Q_PROPERTY(double angle MEMBER angle)
  Q_PROPERTY(int distance MEMBER distance)
  Q_PROPERTY(bool animateAngle MEMBER animateAngle)
  double angle;
  int distance;
  bool animateAngle = true;
};

class UkrugPointsList : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  Q_PROPERTY(UkrugPoint point1 READ getPoint1 NOTIFY pointsChanged)
  Q_PROPERTY(UkrugPoint point2 READ getPoint2 NOTIFY pointsChanged)
  Q_PROPERTY(UkrugPoint point3 READ getPoint3 NOTIFY pointsChanged)
  Q_PROPERTY(UkrugPoint point4 READ getPoint4 NOTIFY pointsChanged)

  UkrugPoint getPoint1() { return _points[0]; }
  UkrugPoint getPoint2() { return _points[1]; }
  UkrugPoint getPoint3() { return _points[2]; }
  UkrugPoint getPoint4() { return _points[3]; }

  Q_INVOKABLE void assignLetter(QString character);

  UkrugPointsList();

  QJsonObject _pointsData;
  QList<UkrugPoint> _points;

  signals:
  void pointsChanged();
};
