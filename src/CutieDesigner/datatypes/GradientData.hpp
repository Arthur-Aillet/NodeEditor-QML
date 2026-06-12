#pragma once

#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

class GradientData : public NodeData {
  public:
  GradientData() {}
  GradientData(QProperty<QGradient> &gradientProp) {
    defineBinding(gradientProp.subscribe(BindingFn([this, &gradientProp]() {
      _map.insert_or_assign(QMetaType::fromType<QGradient>(),
                            QVariant::fromValue(gradientProp.value()));
    })));
  }

  inline static const NodeDataType dataType = NodeDataType("gradient-01", "Gradient");

  const NodeDataType &type() const override { return dataType; }
};
