#include "ObjectData.hpp"

ObjectData::ObjectData(std::unique_ptr<QQmlComponent> &&_object, QVariantMap _initialProps)
    : component(std::move(_object)), initialProps(_initialProps) {}
