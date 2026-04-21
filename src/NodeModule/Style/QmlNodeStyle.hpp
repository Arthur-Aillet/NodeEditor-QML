#pragma once

#include <QObject>
#include <QtNodes/NodeStyle>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

using QtNodes::NodeStyle;

class QmlNodeStyle : public NodeStyle {
  Q_GADGET
  QML_VALUE_TYPE(nodeStyle)

  public:
  QmlNodeStyle() : NodeStyle() {}
  QmlNodeStyle(QString jsonText) : NodeStyle(jsonText) {};
  QmlNodeStyle(QJsonObject const &json) : NodeStyle(json) {};

  Q_PROPERTY(QColor normalBoundaryColor MEMBER NormalBoundaryColor)
};