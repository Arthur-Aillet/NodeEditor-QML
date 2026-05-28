#pragma once

#include "NodeData.hpp"
#include <memory>
#include <qqmlcomponent.h>

class ObjectData : public NodeData {
  public:
  ObjectData() {}
  ObjectData(std::unique_ptr<QQmlComponent> &&_object) : object(std::move(_object)) {}

  static inline const NodeDataType dataType = NodeDataType("object", "2D Object");

  const NodeDataType &type() const override { return dataType; }

  std::unique_ptr<QQmlComponent> object;
};
