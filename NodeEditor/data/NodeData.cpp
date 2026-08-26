#include "NodeData.hpp"

using namespace NodeEditor;

// By default, each type is only compatible with itself
NodeDataType::NodeDataType(DataTypeId _id, QString _name)
    : id(_id), name(_name), compatibleTypes() {
  compatibleTypes.push_back(_id);
}
NodeDataType::NodeDataType(DataTypeId _id, QString _name, std::vector<QString> _compatibleTypes)
    : id(_id), name(_name), compatibleTypes() {
  for (const auto &type : _compatibleTypes) {
    compatibleTypes.push_back(type);
  }
}

bool NodeDataType::operator==(const NodeDataType &other) const {
  return id == other.id && name == other.name && compatibleTypes == other.compatibleTypes;
}