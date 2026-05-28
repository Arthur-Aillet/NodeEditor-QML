#pragma once

#include <QtGui/QColor>
#include <qcolor.h>
#include <qdebug.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvectornd.h>

#include "Style.hpp"

class ConnectionStyle : public Style {
  Q_GADGET
  QML_VALUE_TYPE(connectionStyle)

  public:
  ConnectionStyle();
  ConnectionStyle(QString jsonText);
  ~ConnectionStyle() = default;

  Q_INVOKABLE static void setConnectionStyle(QString jsonText);
  Q_INVOKABLE void loadJson(QJsonObject const &json) override;
  Q_INVOKABLE QJsonObject toJson() const override;

  Q_INVOKABLE QColor lerpOklabColors(const QColor &first, const QColor &second,
                                     const float amount) const;

  Q_PROPERTY(QColor constructionColor READ constructionColor)
  Q_PROPERTY(QColor normalColor READ normalColor)
  Q_PROPERTY(QColor selectedColor READ selectedColor)
  Q_PROPERTY(QColor selectedHaloColor READ selectedHaloColor)
  Q_PROPERTY(QColor hoveredColor READ hoveredColor)

  Q_PROPERTY(float lineWidth READ lineWidth)
  Q_PROPERTY(float constructionLineWidth READ constructionLineWidth)
  Q_PROPERTY(float pointDiameter READ pointDiameter)

  Q_PROPERTY(bool useDataDefinedColors READ useDataDefinedColors)

  QColor constructionColor() const;
  QColor normalColor() const;
  Q_INVOKABLE QColor typeColor(QString typeId) const;
  QColor selectedColor() const;
  QColor selectedHaloColor() const;
  QColor hoveredColor() const;

  float lineWidth() const;
  float constructionLineWidth() const;
  float pointDiameter() const;

  bool useDataDefinedColors() const;

  bool operator==(const ConnectionStyle &other) const;

  protected:
  QColor ConstructionColor;
  QColor NormalColor;
  QColor SelectedColor;
  QColor SelectedHaloColor;
  QColor HoveredColor;

  float LineWidth;
  float ConstructionLineWidth;
  float PointDiameter;

  bool UseDataDefinedColors;
};
