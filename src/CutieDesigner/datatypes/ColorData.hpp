#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>
#include <qcolor.h>
#include <qproperty.h>
#include <qvariant.h>

struct ColorDataType : public NodeDataType {
  ColorDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(GradientData().type().id);
    return types;
  }
};

class ColorData : public NodeData {

  public:
  ColorData() {}
  ColorData(QProperty<QVariant> *colorProp) : _colorProp(colorProp) {
    QGradient gradient = QLinearGradient();
    gradient.setColorAt(0, qvariant_cast<QColor>(_colorProp->value()));
    _gradient = QVariant::fromValue(gradient);

    _registeredBindings.push_back(_colorProp->subscribe(BindingFn([this]() {
      _gradient.value<QGradient>().setColorAt(0, _colorProp->value().value<QColor>());
    })));
  }

  inline static const ColorDataType dataType = ColorDataType("col", "Color");
  const ColorDataType &type() const override { return dataType; }

  const QVariant &get(QMetaType type) const override {
    if (type == QMetaType::fromType<QColor>())
      return _colorProp->value();
    if (type == QMetaType::fromType<QGradient>())
      return _gradient;
    return err;
  }

  protected:
  QProperty<QVariant> *_colorProp;
  QVariant _gradient;
};
