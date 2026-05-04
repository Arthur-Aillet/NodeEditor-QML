#pragma once

#include <QtNodes/ConnectionStyle>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

using QtNodes::ConnectionStyle;

class NODE_EDITOR_PUBLIC QmlConnectionStyle : public ConnectionStyle {
  public:
  Q_GADGET
  QML_VALUE_TYPE(connectionStyle)

  public:
  QmlConnectionStyle() : ConnectionStyle() {}
  QmlConnectionStyle(QString jsonText) : ConnectionStyle(jsonText) {};

  bool operator==(const QmlConnectionStyle &other) {
    return ConstructionColor == other.ConstructionColor && NormalColor == other.NormalColor &&
           SelectedColor == other.SelectedColor;
  }

  public:
  Q_INVOKABLE void loadJson(QJsonObject const &json) { ConnectionStyle::loadJson(json); };

  Q_PROPERTY(QColor constructionColor MEMBER ConstructionColor)
  Q_PROPERTY(QColor normalColor MEMBER NormalColor)
  Q_PROPERTY(QColor selectedColor MEMBER SelectedColor)
  Q_PROPERTY(QColor selectedHaloColor MEMBER SelectedHaloColor)
  Q_PROPERTY(QColor hoveredColor MEMBER HoveredColor)

  Q_PROPERTY(float lineWidth MEMBER LineWidth)
  Q_PROPERTY(float constructionLineWidth MEMBER ConstructionLineWidth)
  Q_PROPERTY(float pointDiameter MEMBER PointDiameter)

  Q_PROPERTY(bool useDataDefinedColors MEMBER UseDataDefinedColors)
};
