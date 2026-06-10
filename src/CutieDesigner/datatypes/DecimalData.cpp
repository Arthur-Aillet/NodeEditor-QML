#include "DecimalData.hpp"
#include "TextData.hpp"

QList<NodeDataType::DataTypeId> DecimalDataType::compatibleTypes() const {
  QList<NodeDataType::DataTypeId> types;
  types.push_front(id);
  types.push_front(TextData().type().id);
  return types;
}