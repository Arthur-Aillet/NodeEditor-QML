#pragma once

#include <QtGui/QColor>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

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

  Q_PROPERTY(QColor constructionColor MEMBER _constructionColor)
  Q_PROPERTY(QColor normalColor MEMBER _normalColor)
  Q_PROPERTY(QColor selectedColor MEMBER _selectedColor)
  Q_PROPERTY(QColor selectedHaloColor MEMBER _selectedHaloColor)
  Q_PROPERTY(QColor hoveredColor MEMBER _hoveredColor)

  Q_PROPERTY(float lineWidth MEMBER _lineWidth)
  Q_PROPERTY(float constructionLineWidth MEMBER _constructionLineWidth)
  Q_PROPERTY(float pointDiameter MEMBER _pointDiameter)

  Q_PROPERTY(bool useDataDefinedColors MEMBER _useDataDefinedColors)

  QColor getNormalColor(QString typeId) const;

  bool operator==(const ConnectionStyle &other);

  protected:
  QColor _constructionColor;
  QColor _normalColor;
  QColor _selectedColor;
  QColor _selectedHaloColor;
  QColor _hoveredColor;

  float _lineWidth;
  float _constructionLineWidth;
  float _pointDiameter;

  bool _useDataDefinedColors;
};
