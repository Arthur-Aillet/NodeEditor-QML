#pragma once

#include <QtGui/QColor>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

#include "Style.hpp"

class GraphicsViewStyle : public Style {
  Q_GADGET
  QML_VALUE_TYPE(graphicsViewStyle)

  public:
  GraphicsViewStyle();

  GraphicsViewStyle(QString jsonText);

  ~GraphicsViewStyle() = default;

  public:
  Q_INVOKABLE static void setStyle(QString jsonText);
  Q_INVOKABLE void loadJson(QJsonObject const &json) override;
  Q_INVOKABLE QJsonObject toJson() const override;

  QColor BackgroundColor;
  QColor FineGridColor;
  QColor CoarseGridColor;

  Q_PROPERTY(QColor backgroundColor MEMBER BackgroundColor)
  Q_PROPERTY(QColor fineGridColor MEMBER FineGridColor)
  Q_PROPERTY(QColor coarseGridColor MEMBER CoarseGridColor)

  bool operator==(const GraphicsViewStyle &other);
};
