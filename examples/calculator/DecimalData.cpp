#include "DecimalData.hpp"
#include "NodeData.hpp"

DecimalDataType::DecimalDataType() : NodeDataType("decimal", "Decimal") {}

DecimalData::DecimalData(const double &v) {
  registerConvert([&v]() { return v; });
}