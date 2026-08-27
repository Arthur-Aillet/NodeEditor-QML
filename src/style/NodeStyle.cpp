#include "NodeStyle.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>

using namespace NodeEditor;

// inline void initResources() { Q_INIT_RESOURCE(resources); }

NodeStyle NodeStyle::defaultStyle() {
  NodeStyle newStyle;

  // Explicit resources inialization for preventing the static initialization
  // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
  // initResources();

  // Initialize status icons after resources are loaded
  newStyle.statusUpdated = QIcon(":/NodeEditor/status_icons/updated.svg");
  newStyle.statusProcessing = QIcon(":/NodeEditor/status_icons/processing.svg");
  newStyle.statusPending = QIcon(":/NodeEditor/status_icons/pending.svg");
  newStyle.statusInvalid = QIcon(":/NodeEditor/status_icons/failed.svg");
  newStyle.statusEmpty = QIcon(":/NodeEditor/status_icons/empty.svg");
  newStyle.statusPartial = QIcon(":/NodeEditor/status_icons/partial.svg");

  // This configuration is stored inside the compiled unit and is loaded statically
  newStyle.loadJsonFile(":/NodeEditor/DefaultStyle.json");
  return newStyle;
}

NodeStyle::NodeStyle(QPalette const &palette) {
  // Initialize status icons after resources are loaded
  statusUpdated = QIcon(":/NodeEditor/status_icons/updated.svg");
  statusProcessing = QIcon(":/NodeEditor/status_icons/processing.svg");
  statusPending = QIcon(":/NodeEditor/status_icons/pending.svg");
  statusInvalid = QIcon(":/NodeEditor/status_icons/failed.svg");
  statusEmpty = QIcon(":/NodeEditor/status_icons/empty.svg");
  statusPartial = QIcon(":/NodeEditor/status_icons/partial.svg");

  // This configuration is stored inside the compiled unit and is loaded statically
  loadJsonFile(":/NodeEditor/DefaultStyle.json");
  loadPalette(palette);
}

NodeStyle::NodeStyle(QString jsonText) { loadJsonText(jsonText); }

NodeStyle::NodeStyle(QJsonObject const &json) { loadJson(json); }

#ifdef STYLE_DEBUG
#define NODE_STYLE_CHECK_UNDEFINED_VALUE(v, variable)                                              \
  {                                                                                                \
    if (v.type() == QJsonValue::Undefined || v.type() == QJsonValue::Null)                         \
      qWarning() << "Undefined value for parameter:" << #variable;                                 \
  }
#else
#define NODE_STYLE_CHECK_UNDEFINED_VALUE(v, variable)
#endif

#define NODE_STYLE_READ_COLOR(values, variable)                                                    \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                           \
    if (valueRef.isArray()) {                                                                      \
      auto colorArray = valueRef.toArray();                                                        \
      std::vector<int> rgb;                                                                        \
      rgb.reserve(3);                                                                              \
      for (auto it = colorArray.begin(); it != colorArray.end(); ++it) {                           \
        rgb.push_back((*it).toInt());                                                              \
      }                                                                                            \
      variable = QColor(rgb[0], rgb[1], rgb[2]);                                                   \
    } else {                                                                                       \
      variable = QColor(valueRef.toString());                                                      \
    }                                                                                              \
  }

#define NODE_STYLE_WRITE_COLOR(values, variable)                                                   \
  {                                                                                                \
    values[#variable] = variable.name();                                                           \
  }

#define NODE_STYLE_READ_FLOAT(values, variable)                                                    \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                           \
    variable = valueRef.toDouble();                                                                \
  }

#define NODE_STYLE_WRITE_FLOAT(values, variable)                                                   \
  {                                                                                                \
    values[#variable] = variable;                                                                  \
  }

#define NODE_STYLE_READ_BOOL(values, variable)                                                     \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                           \
    variable = valueRef.toBool();                                                                  \
  }

#define NODE_STYLE_WRITE_BOOL(values, variable)                                                    \
  {                                                                                                \
    values[#variable] = variable;                                                                  \
  }

void NodeStyle::loadJson(QJsonObject const &json) {
  QJsonValue nodeStyleValues = json["NodeStyle"];

  QJsonObject obj = nodeStyleValues.toObject();

  NODE_STYLE_READ_COLOR(obj, NormalBoundaryColor);
  NODE_STYLE_READ_COLOR(obj, SelectedBoundaryColor);
  NODE_STYLE_READ_COLOR(obj, GradientColor0);
  NODE_STYLE_READ_COLOR(obj, GradientColor1);
  NODE_STYLE_READ_COLOR(obj, GradientColor2);
  NODE_STYLE_READ_COLOR(obj, GradientColor3);
  NODE_STYLE_READ_COLOR(obj, ShadowColor);
  NODE_STYLE_READ_BOOL(obj, ShadowEnabled);
  NODE_STYLE_READ_COLOR(obj, FontColor);
  NODE_STYLE_READ_COLOR(obj, FontColorFaded);
  NODE_STYLE_READ_COLOR(obj, ConnectionPointColor);
  NODE_STYLE_READ_COLOR(obj, FilledConnectionPointColor);
  NODE_STYLE_READ_COLOR(obj, WarningColor);
  NODE_STYLE_READ_COLOR(obj, ErrorColor);

  NODE_STYLE_READ_FLOAT(obj, PenWidth);
  NODE_STYLE_READ_FLOAT(obj, HoveredPenWidth);
  NODE_STYLE_READ_FLOAT(obj, ConnectionPointDiameter);

  NODE_STYLE_READ_FLOAT(obj, Opacity);
}

QJsonObject NodeStyle::toJson() const {
  QJsonObject obj;

  NODE_STYLE_WRITE_COLOR(obj, NormalBoundaryColor);
  NODE_STYLE_WRITE_COLOR(obj, SelectedBoundaryColor);
  NODE_STYLE_WRITE_COLOR(obj, GradientColor0);
  NODE_STYLE_WRITE_COLOR(obj, GradientColor1);
  NODE_STYLE_WRITE_COLOR(obj, GradientColor2);
  NODE_STYLE_WRITE_COLOR(obj, GradientColor3);
  NODE_STYLE_WRITE_COLOR(obj, ShadowColor);
  NODE_STYLE_WRITE_BOOL(obj, ShadowEnabled);
  NODE_STYLE_WRITE_COLOR(obj, FontColor);
  NODE_STYLE_WRITE_COLOR(obj, FontColorFaded);
  NODE_STYLE_WRITE_COLOR(obj, ConnectionPointColor);
  NODE_STYLE_WRITE_COLOR(obj, FilledConnectionPointColor);
  NODE_STYLE_WRITE_COLOR(obj, WarningColor);
  NODE_STYLE_WRITE_COLOR(obj, ErrorColor);

  NODE_STYLE_WRITE_FLOAT(obj, PenWidth);
  NODE_STYLE_WRITE_FLOAT(obj, HoveredPenWidth);
  NODE_STYLE_WRITE_FLOAT(obj, ConnectionPointDiameter);

  NODE_STYLE_WRITE_FLOAT(obj, Opacity);

  QJsonObject root;
  root["NodeStyle"] = obj;

  return root;
}

void NodeStyle::loadPalette(QPalette const &palette) {
  loadJsonFile(":/NodeEditor/DefaultStyle.json");

  SelectedBoundaryColor = palette.highlight().color();
  FontColor = palette.text().color();
  FontColorFaded = palette.text().color().darker(120);
  ShadowColor = palette.shadow().color();

  if (palette.text().color().lightnessF() > 0.5) { // Darker themes
    NormalBoundaryColor =
        palette.dark().color().darker(140); // palette.light().color().lighter(240);
    GradientColor0 = palette.base().color().lighter().lighter(150);
    GradientColor1 = palette.base().color().lighter().lighter(100);
    GradientColor2 = palette.base().color().lighter().darker(120);
    GradientColor3 = palette.base().color().lighter().darker(140);
  } else {
    NormalBoundaryColor = palette.highlight().color().darker(170);
    GradientColor0 = palette.alternateBase().color().lighter(150);
    GradientColor1 = palette.alternateBase().color().lighter(100);
    GradientColor2 = palette.alternateBase().color().darker(110);
    GradientColor3 = palette.alternateBase().color().darker(125);
  }
}

QColor NodeStyle::normalBoundaryColor() const { return NormalBoundaryColor; };
QColor NodeStyle::selectedBoundaryColor() const { return SelectedBoundaryColor; };
QColor NodeStyle::gradientColor0() const { return GradientColor0; };
QColor NodeStyle::gradientColor1() const { return GradientColor1; };
QColor NodeStyle::gradientColor2() const { return GradientColor2; };
QColor NodeStyle::gradientColor3() const { return GradientColor3; };
QColor NodeStyle::shadowColor() const { return ShadowColor; };

bool NodeStyle::shadowEnabled() const { return ShadowEnabled; };

QColor NodeStyle::fontColor() const { return FontColor; };
QColor NodeStyle::fontColorFaded() const { return FontColorFaded; };

QColor NodeStyle::connectionPointColor() const { return ConnectionPointColor; };
QColor NodeStyle::filledConnectionPointColor() const { return FilledConnectionPointColor; };

QColor NodeStyle::warningColor() const { return WarningColor; };
QColor NodeStyle::errorColor() const { return ErrorColor; };
QColor NodeStyle::toolTipIconColor() const { return ToolTipIconColor; };

float NodeStyle::penWidth() const { return PenWidth; };
float NodeStyle::hoveredPenWidth() const { return HoveredPenWidth; };
float NodeStyle::connectionPointDiameter() const { return ConnectionPointDiameter; };

float NodeStyle::opacity() const { return Opacity; };

void NodeStyle::setBackgroundColor(QColor const &color) {
  GradientColor0 = color;
  GradientColor1 = color;
  GradientColor2 = color;
  GradientColor3 = color;
}

QColor NodeStyle::backgroundColor() const { return GradientColor0; }
