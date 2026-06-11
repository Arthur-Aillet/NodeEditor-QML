#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
#include <functional>
#include <memory>
#include <qbrush.h>
#include <qproperty.h>

struct ColorDataType : public NodeDataType {
  ColorDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(GradientData().type().id);
    return types;
  }
};

class ColorData : public GradientData {
  typedef QPropertyChangeHandler<std::function<void(void)>> colorChange;

  public:
  ColorData() {}
  ColorData(QProperty<QColor> *colorProp) : _colorProp(colorProp), GradientData(QLinearGradient()) {
    _gradient.setColorAt(0, _colorProp->value());
    sub = std::make_unique<colorChange>(_colorProp->subscribe(
        std::function<void(void)>([this]() { _gradient.setColorAt(0, _colorProp->value()); })));
  }

  inline static const ColorDataType dataType = ColorDataType("col", "Color");

  const ColorDataType &type() const override { return dataType; }

  QColor color() { return _colorProp->value(); }

  protected:
  std::unique_ptr<colorChange> sub;
  QProperty<QColor> *_colorProp;
};
