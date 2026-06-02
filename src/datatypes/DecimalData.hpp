#pragma once

#include "NodeData.hpp"
#include "TextData.hpp"
#include <memory>

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class DecimalData : public NodeData {
  public:
  DecimalData(double const number = 0.0)
      : _number(number), _textRepr(std::make_shared<TextData>(QString::number(number, 'f', 2))) {}

  inline static const NodeDataType dataType = NodeDataType("decimal", "Decimal");

  const NodeDataType &type() const override { return dataType; }

  double number() const { return _number; }

  std::shared_ptr<TextData> numberAsText() const { return _textRepr; }

  private:
  double _number;
  std::shared_ptr<TextData> _textRepr;
};
