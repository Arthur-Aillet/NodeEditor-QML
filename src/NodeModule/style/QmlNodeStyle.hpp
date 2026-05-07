#pragma once

#include "NodeStyle.hpp"
#include <QObject>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class QmlNodeStyle : public NodeStyle {
  Q_GADGET
  QML_VALUE_TYPE(nodeStyle)

  public:
  QmlNodeStyle() : NodeStyle() {}
  QmlNodeStyle(QString jsonText) : NodeStyle(jsonText) {};
  QmlNodeStyle(QJsonObject const &json) : NodeStyle(json) {};

  public:
  Q_INVOKABLE void loadJson(QJsonObject const &json) { NodeStyle::loadJson(json); };

  Q_PROPERTY(QColor normalBoundaryColor MEMBER NormalBoundaryColor)
  Q_PROPERTY(QColor selectedBoundaryColor MEMBER SelectedBoundaryColor)
  Q_PROPERTY(QColor gradientColor0 MEMBER GradientColor0)
  Q_PROPERTY(QColor gradientColor1 MEMBER GradientColor1)
  Q_PROPERTY(QColor gradientColor2 MEMBER GradientColor2)
  Q_PROPERTY(QColor gradientColor3 MEMBER GradientColor3)
  Q_PROPERTY(QColor shadowColor MEMBER ShadowColor)

  Q_PROPERTY(bool shadowEnabled MEMBER ShadowEnabled)

  Q_PROPERTY(QColor fontColor MEMBER FontColor)
  Q_PROPERTY(QColor fontColorFaded MEMBER FontColorFaded)

  Q_PROPERTY(QColor connectionPointColor MEMBER ConnectionPointColor)
  Q_PROPERTY(QColor filledConnectionPointColor MEMBER FilledConnectionPointColor)

  Q_PROPERTY(QColor warningColor MEMBER WarningColor)
  Q_PROPERTY(QColor errorColor MEMBER ErrorColor)
  Q_PROPERTY(QColor toolTipIconColor MEMBER ToolTipIconColor)

  Q_PROPERTY(float penWidth MEMBER PenWidth)
  Q_PROPERTY(float hoveredPenWidth MEMBER HoveredPenWidth)

  Q_PROPERTY(float connectionPointDiameter MEMBER ConnectionPointDiameter)

  Q_PROPERTY(float opacity MEMBER Opacity)

  // Var should be captialize, todo
  //  Q_PROPERTY(QIcon statusUpdated MEMBER statusUpdated)
  //  Q_PROPERTY(QIcon statusProcessing MEMBER statusProcessing)
  //  Q_PROPERTY(QIcon statusPending MEMBER statusPending)
  //  Q_PROPERTY(QIcon statusInvalid MEMBER statusInvalid)
  //  Q_PROPERTY(QIcon statusEmpty MEMBER statusEmpty)
  //  Q_PROPERTY(QIcon statusPartial MEMBER statusPartial)
};