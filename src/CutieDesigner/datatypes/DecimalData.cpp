#include "DecimalData.hpp"
#include "ColorData.hpp"
#include "GradientData.hpp"
#include "NodeData.hpp"
#include "TextData.hpp"
#include "Vec2Data.hpp"

DecimalDataType::DecimalDataType()
    : NodeDataType("decimal", "Decimal", {"decimal", "text", "col", "gradient-01", "vec2-01"}) {}

DecimalData::DecimalData(const double &v) {
  registerConvert([&v]() { return v; });
  registerConvert([&v]() { return QString::number(v, 'f', 2); });
  registerConvert([&v]() {
    return QColor::fromRgbF(std::clamp(v, 0.0, 1.0), std::clamp(v, 0.0, 1.0),
                            std::clamp(v, 0.0, 1.0), 1);
  });
  registerConvert([&v]() {
    QColor col = QColor::fromRgbF(std::clamp(v, 0.0, 1.0), std::clamp(v, 0.0, 1.0),
                                  std::clamp(v, 0.0, 1.0), 1);
    QGradient g;
    g.setColorAt(0, col);
    return g;
  });
  registerConvert([&v]() { return QVector2D(v, v); });
}