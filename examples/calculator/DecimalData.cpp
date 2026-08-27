#include "DecimalData.hpp"

DecimalDataType::DecimalDataType() : NodeDataType("decimal", "Decimal") {}

DecimalData::DecimalData(const double &v) {
  registerConvert([&v]() { return v; });
}
