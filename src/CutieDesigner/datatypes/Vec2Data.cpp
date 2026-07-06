#include "Vec2Data.hpp"
#include "DecimalData.hpp"

Vec2DataType::Vec2DataType() : NodeDataType("vec2-01", "Vec2", {"vec2-01", "decimal"}) {}

Vec2Data::Vec2Data(const QVector2D &v) {
  registerConvert([&v]() { return v; });
  registerConvert([&v]() { return v.length(); });
}
