#pragma once

#include "UkrugNode.hpp"

struct UkrugPoint {
  Q_GADGET
  QML_VALUE_TYPE(ukrugPoint)
  QML_STRUCTURED_VALUE

  public:
  Q_PROPERTY(double angle MEMBER angle)
  Q_PROPERTY(double distance MEMBER distance)
  Q_PROPERTY(bool animateAngle MEMBER animateAngle)
  double angle;
  double distance;
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

  Q_PROPERTY(double defaultOutsideDist MEMBER defaultOutsideDist CONSTANT)

  UkrugPoint getPoint1() { return _points[0]; }
  UkrugPoint getPoint2() { return _points[1]; }
  UkrugPoint getPoint3() { return _points[2]; }
  UkrugPoint getPoint4() { return _points[3]; }

  Q_INVOKABLE bool assignLetter(QString character);

  UkrugPointsList();

  QJsonObject _pointsData;
  QList<UkrugPoint> _points;

  double defaultOutsideDist = 4.5;

  private:
  QColor _baseColor = "white";
  double _fontSize = 150;
  double _animationOpacitySpeed = 250;
  double _animationWidthSpeed = 100;

  public:
  signals:
  void pointsChanged();
};
