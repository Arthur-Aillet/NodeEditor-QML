#pragma once

#include "Style.hpp"

#include <QIcon>
#include <QtGui/QColor>
#include <QtQmlIntegration>
#include <qpalette.h>

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
  NodeStyle(QPalette const &palette);
  static NodeStyle defaultStyle();
  NodeStyle(QString jsonText);
  NodeStyle(QJsonObject const &json);

  virtual ~NodeStyle() = default;

  Q_INVOKABLE static void setNodeStyle(QString jsonText);
  Q_INVOKABLE void loadJson(QJsonObject const &json) override;
  Q_INVOKABLE void loadPalette(QPalette const &palette) override;

  Q_INVOKABLE QJsonObject toJson() const override;

  bool operator==(const NodeStyle &other) { return toJson() == other.toJson(); }

  /// Set uniform background color for the node.
  void setBackgroundColor(QColor const &color);

  /// Current uniform background color.
  QColor backgroundColor() const;

  Q_PROPERTY(QColor normalBoundaryColor READ normalBoundaryColor)
  Q_PROPERTY(QColor selectedBoundaryColor READ selectedBoundaryColor)
  Q_PROPERTY(QColor gradientColor0 READ gradientColor0)
  Q_PROPERTY(QColor gradientColor1 READ gradientColor1)
  Q_PROPERTY(QColor gradientColor2 READ gradientColor2)
  Q_PROPERTY(QColor gradientColor3 READ gradientColor3)
  Q_PROPERTY(QColor shadowColor READ shadowColor)

  Q_PROPERTY(bool shadowEnabled READ shadowEnabled)

  Q_PROPERTY(QColor fontColor READ fontColor)
  Q_PROPERTY(QColor fontColorFaded READ fontColorFaded)

  Q_PROPERTY(QColor connectionPointColor READ connectionPointColor)
  Q_PROPERTY(QColor filledConnectionPointColor READ filledConnectionPointColor)

  Q_PROPERTY(QColor warningColor READ warningColor)
  Q_PROPERTY(QColor errorColor READ errorColor)
  Q_PROPERTY(QColor toolTipIconColor READ toolTipIconColor)

  Q_PROPERTY(float penWidth READ penWidth)
  Q_PROPERTY(float hoveredPenWidth READ hoveredPenWidth)

  Q_PROPERTY(float connectionPointDiameter READ connectionPointDiameter)

  Q_PROPERTY(float opacity READ opacity)

  // Q_PROPERTY(QIcon statusUpdated READ statusUpdated)
  // Q_PROPERTY(QIcon statusProcessing READ statusProcessing)
  // Q_PROPERTY(QIcon statusPending READ statusPending)
  // Q_PROPERTY(QIcon statusInvalid READ statusInvalid)
  // Q_PROPERTY(QIcon statusEmpty READ statusEmpty)
  // Q_PROPERTY(QIcon statusPartial READ statusPartial)
  // Status icons - initialized in constructor after Q_INIT_RESOURCE
  QColor normalBoundaryColor() const;
  QColor selectedBoundaryColor() const;
  QColor gradientColor0() const;
  QColor gradientColor1() const;
  QColor gradientColor2() const;
  QColor gradientColor3() const;
  QColor shadowColor() const;

  bool shadowEnabled() const;

  QColor fontColor() const;
  QColor fontColorFaded() const;

  QColor connectionPointColor() const;
  QColor filledConnectionPointColor() const;

  QColor warningColor() const;
  QColor errorColor() const;
  QColor toolTipIconColor() const;

  float penWidth() const;
  float hoveredPenWidth() const;
  float connectionPointDiameter() const;

  float opacity() const;

  QIcon statusUpdated;
  QIcon statusProcessing;
  QIcon statusPending;
  QIcon statusInvalid;
  QIcon statusEmpty;
  QIcon statusPartial;

  ProcessingIconStyle processingIconStyle{};

  protected:
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
};