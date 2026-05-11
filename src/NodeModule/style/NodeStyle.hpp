#pragma once

#include <QIcon>
#include <QtGui/QColor>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

#include "Style.hpp"

/**
 * Describes the position of the processing icon on the node ui
 */
enum class ProcessingIconPos {
  BottomLeft = 0,  /// icon on the bottom left position
  BottomRight = 1, /// icon on the bottom right position
};

/**
 * Defines the processing icon style;
 */
struct ProcessingIconStyle {
  ProcessingIconPos _pos{ProcessingIconPos::BottomRight};
  double _size{20.0};
  double _margin{8.0};
  int _resolution{64};
};

class NodeStyle : public Style {
  Q_GADGET
  QML_VALUE_TYPE(nodeStyle)

  public:
  NodeStyle() = default;
  static NodeStyle defaultStyle();
  NodeStyle(QString jsonText);
  NodeStyle(QJsonObject const &json);

  virtual ~NodeStyle() = default;

  Q_INVOKABLE static void setNodeStyle(QString jsonText);
  Q_INVOKABLE void loadJson(QJsonObject const &json) override;

  Q_INVOKABLE QJsonObject toJson() const override;

  bool operator==(const NodeStyle &other) { return toJson() == other.toJson(); }

  /// Set uniform background color for the node.
  void setBackgroundColor(QColor const &color);

  /// Current uniform background color.
  QColor backgroundColor() const;

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

  // Q_PROPERTY(QIcon statusUpdated MEMBER StatusUpdated)
  // Q_PROPERTY(QIcon statusProcessing MEMBER StatusProcessing)
  // Q_PROPERTY(QIcon statusPending MEMBER StatusPending)
  // Q_PROPERTY(QIcon statusInvalid MEMBER StatusInvalid)
  // Q_PROPERTY(QIcon statusEmpty MEMBER StatusEmpty)
  // Q_PROPERTY(QIcon statusPartial MEMBER StatusPartial)

  public:
  QColor NormalBoundaryColor;
  QColor SelectedBoundaryColor;
  QColor GradientColor0;
  QColor GradientColor1;
  QColor GradientColor2;
  QColor GradientColor3;
  QColor ShadowColor;
  bool ShadowEnabled;
  QColor FontColor;
  QColor FontColorFaded;

  QColor ConnectionPointColor;
  QColor FilledConnectionPointColor;

  QColor WarningColor;
  QColor ErrorColor;
  QColor ToolTipIconColor;

  float PenWidth;
  float HoveredPenWidth;

  float ConnectionPointDiameter;

  float Opacity;

  // Status icons - initialized in constructor after Q_INIT_RESOURCE
  QIcon statusUpdated;
  QIcon statusProcessing;
  QIcon statusPending;
  QIcon statusInvalid;
  QIcon statusEmpty;
  QIcon statusPartial;

  ProcessingIconStyle processingIconStyle{};
};