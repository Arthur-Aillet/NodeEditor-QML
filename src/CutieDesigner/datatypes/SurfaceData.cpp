#include "SurfaceData.hpp"

SurfaceData::SurfaceData(std::unique_ptr<QQmlComponent> &&object, QVariantMap initialProps)
    : _component(std::move(object)), _initialProps(initialProps) {}
