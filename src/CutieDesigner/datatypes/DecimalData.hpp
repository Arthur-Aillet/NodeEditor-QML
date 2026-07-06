#pragma once

#include "NodeData.hpp"
#include <qbrush.h>
#include <qvariant.h>
#include <qvectornd.h>

struct DecimalDataType : public NodeDataType {
  DecimalDataType();
};

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(const double &v);

  NodeDataType type() const override { return DecimalDataType(); }
};
