#include "GraphicsViewStyle.hpp"

#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
#include <QtWidgets/QApplication>

// inline void initResources() { Q_INIT_RESOURCE(resources); }

GraphicsViewStyle::GraphicsViewStyle() {
  // Explicit resources inialization for preventing the static initialization
  // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
  // initResources();

  // This configuration is stored inside the compiled unit and is loaded statically
  loadJsonFile(":/NodeEditor/DefaultStyle.json");
}

GraphicsViewStyle::GraphicsViewStyle(QPalette const &palette) { loadPalette(palette); }
GraphicsViewStyle::GraphicsViewStyle(QString jsonText) { loadJsonText(jsonText); }

#ifdef STYLE_DEBUG
#define FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(v, variable)                                         \
  {                                                                                                \
    if (v.type() == QJsonValue::Undefined || v.type() == QJsonValue::Null)                         \
      qWarning() << "Undefined value for parameter:" << #variable;                                 \
  }
#else
#define FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(v, variable)
#endif

#define FLOW_VIEW_STYLE_READ_COLOR(values, variable)                                               \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                      \
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

#define FLOW_VIEW_STYLE_WRITE_COLOR(values, variable)                                              \
  {                                                                                                \
    values[#variable] = variable.name();                                                           \
  }

void GraphicsViewStyle::loadJson(QJsonObject const &json) {
  QJsonValue nodeStyleValues = json["GraphicsViewStyle"];

  QJsonObject obj = nodeStyleValues.toObject();

  FLOW_VIEW_STYLE_READ_COLOR(obj, BackgroundColor);
  FLOW_VIEW_STYLE_READ_COLOR(obj, FineGridColor);
  FLOW_VIEW_STYLE_READ_COLOR(obj, CoarseGridColor);
}

QJsonObject GraphicsViewStyle::toJson() const {
  QJsonObject obj;

  FLOW_VIEW_STYLE_WRITE_COLOR(obj, BackgroundColor);
  FLOW_VIEW_STYLE_WRITE_COLOR(obj, FineGridColor);
  FLOW_VIEW_STYLE_WRITE_COLOR(obj, CoarseGridColor);

  QJsonObject root;
  root["GraphicsViewStyle"] = obj;

  return root;
}

void GraphicsViewStyle::loadPalette(QPalette const &palette) {
  BackgroundColor = palette.base().color();
  FineGridColor = palette.midlight().color();
  CoarseGridColor = palette.dark().color();
}

QColor GraphicsViewStyle::backgroundColor() const { return BackgroundColor; }
QColor GraphicsViewStyle::fineGridColor() const { return FineGridColor; }
QColor GraphicsViewStyle::coarseGridColor() const { return CoarseGridColor; }

bool GraphicsViewStyle::operator==(const GraphicsViewStyle &other) const {
  return BackgroundColor == other.BackgroundColor && FineGridColor == other.FineGridColor &&
         CoarseGridColor == other.CoarseGridColor;
}