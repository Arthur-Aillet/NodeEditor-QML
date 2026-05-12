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

  Q_PROPERTY(QColor backgroundColor READ backgroundColor)
  Q_PROPERTY(QColor fineGridColor READ fineGridColor)
  Q_PROPERTY(QColor coarseGridColor READ coarseGridColor)

  QColor backgroundColor() const;
  QColor fineGridColor() const;
  QColor coarseGridColor() const;

  bool operator==(const GraphicsViewStyle &other) const;

  protected:
  QColor BackgroundColor;
  QColor FineGridColor;
  QColor CoarseGridColor;
};
