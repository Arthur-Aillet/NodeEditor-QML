#pragma once

#include "ColorData.hpp"
#include "GradientData.hpp"
#include "NodeData.hpp"
#include "TextData.hpp"
#include <qbrush.h>
#include <qvariant.h>
#include <qvectornd.h>

struct DecimalDataType : public NodeDataType {
  DecimalDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(TextData().type().id);
    types.push_front(ColorData().type().id);
    types.push_front(GradientData().type().id);
    types.push_front("vec2-01"); // Vec2Data().type().id);
    return types;
  }
};

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(const double &v) {
    registerConvert<double>([&v]() { return v; });
    registerConvert<QString>([&v]() { return QString::number(v, 'f', 2); });
    registerConvert<QColor>([&v]() {
      return QColor::fromRgbF(std::clamp(v, 0.0, 1.0), std::clamp(v, 0.0, 1.0),
                              std::clamp(v, 0.0, 1.0), 1);
    });
    registerConvert<QGradient>([&v]() {
      QColor col = QColor::fromRgbF(std::clamp(v, 0.0, 1.0), std::clamp(v, 0.0, 1.0),
                                    std::clamp(v, 0.0, 1.0), 1);
      QGradient g;
      g.setColorAt(0, col);
      return g;
    });
    registerConvert<QVector2D>([&v]() { return QVector2D(v, v); });
  }

  inline static const DecimalDataType dataType = DecimalDataType("decimal", "Decimal");

  const DecimalDataType &type() const override { return dataType; }
};
