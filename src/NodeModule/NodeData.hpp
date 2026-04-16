#pragma once

#include <QtNodes/NodeData>

class NumberData : public QtNodes::NodeData {
  public:
  NumberData(double value = 0.0) : _value(value) {}

  // Unique type identifier
  [[nodiscard]] QtNodes::NodeDataType type() const override { return {"number", "Number"}; }

  [[nodiscard]] double value() const { return _value; }

  private:
  double _value;
};