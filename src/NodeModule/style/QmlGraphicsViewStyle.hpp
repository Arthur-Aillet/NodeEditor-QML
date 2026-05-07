#pragma once

#include "GraphicsViewStyle.hpp"
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class QmlGraphicsViewStyle : public GraphicsViewStyle {
  public:
  Q_GADGET
  QML_VALUE_TYPE(graphicsViewStyle)

  public:
  QmlGraphicsViewStyle() : GraphicsViewStyle() {}
  QmlGraphicsViewStyle(QString jsonText) : GraphicsViewStyle(jsonText) {};

  bool operator==(const QmlGraphicsViewStyle &other) {
    return BackgroundColor == other.BackgroundColor && FineGridColor == other.FineGridColor &&
           CoarseGridColor == other.CoarseGridColor;
  }

  public:
  Q_INVOKABLE void loadJson(QJsonObject const &json) { GraphicsViewStyle::loadJson(json); };

  public:
  Q_PROPERTY(QColor backgroundColor MEMBER BackgroundColor)
  Q_PROPERTY(QColor fineGridColor MEMBER FineGridColor)
  Q_PROPERTY(QColor coarseGridColor MEMBER CoarseGridColor)
};
