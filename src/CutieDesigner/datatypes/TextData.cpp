#include "TextData.hpp"
#include "DecimalData.hpp"

QList<NodeDataType::DataTypeId> TextDataType::compatibleTypes() const {
  QList<NodeDataType::DataTypeId> types;
  types.push_front(id);
  types.push_front(DecimalData().type().id);
  return types;
}