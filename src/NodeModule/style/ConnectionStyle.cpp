#include "ConnectionStyle.hpp"

#include "StyleCollection.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>

#include <QDebug>

#include <random>

// inline void initResources() { Q_INIT_RESOURCE(resources); }

ConnectionStyle::ConnectionStyle() {
  // Explicit resources inialization for preventing the static initialization
  // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
  // initResources();

  // This configuration is stored inside the compiled unit and is loaded statically
  loadJsonFile(":DefaultStyle.json");
}

ConnectionStyle::ConnectionStyle(QString jsonText) {
  loadJsonFile(":DefaultStyle.json");
  loadJsonText(jsonText);
}

void ConnectionStyle::setConnectionStyle(QString jsonText) {
  ConnectionStyle style(jsonText);

  StyleCollection::setConnectionStyle(style);
}

#ifdef STYLE_DEBUG
#define CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(v, variable)                                        \
  {                                                                                                \
    if (v.type() == QJsonValue::Undefined || v.type() == QJsonValue::Null)                         \
      qWarning() << "Undefined value for parameter:" << #variable;                                 \
  }
#else
#define CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(v, variable)
#endif

#define CONNECTION_VALUE_EXISTS(v)                                                                 \
  (v.type() != QJsonValue::Undefined && v.type() != QJsonValue::Null)

#define CONNECTION_STYLE_READ_COLOR(values, variable)                                              \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                     \
    if (CONNECTION_VALUE_EXISTS(valueRef)) {                                                       \
      if (valueRef.isArray()) {                                                                    \
        auto colorArray = valueRef.toArray();                                                      \
        std::vector<int> rgb;                                                                      \
        rgb.reserve(3);                                                                            \
        for (auto it = colorArray.begin(); it != colorArray.end(); ++it) {                         \
          rgb.push_back((*it).toInt());                                                            \
        }                                                                                          \
        variable = QColor(rgb[0], rgb[1], rgb[2]);                                                 \
      } else {                                                                                     \
        variable = QColor(valueRef.toString());                                                    \
      }                                                                                            \
    }                                                                                              \
  }

#define CONNECTION_STYLE_WRITE_COLOR(values, variable)                                             \
  {                                                                                                \
    values[#variable] = variable.name();                                                           \
  }

#define CONNECTION_STYLE_READ_FLOAT(values, variable)                                              \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                     \
    if (CONNECTION_VALUE_EXISTS(valueRef))                                                         \
      variable = valueRef.toDouble();                                                              \
  }

#define CONNECTION_STYLE_WRITE_FLOAT(values, variable)                                             \
  {                                                                                                \
    values[#variable] = variable;                                                                  \
  }

#define CONNECTION_STYLE_READ_BOOL(values, variable)                                               \
  {                                                                                                \
    auto valueRef = values[#variable];                                                             \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable)                                     \
    if (CONNECTION_VALUE_EXISTS(valueRef))                                                         \
      variable = valueRef.toBool();                                                                \
  }

#define CONNECTION_STYLE_WRITE_BOOL(values, variable)                                              \
  {                                                                                                \
    values[#variable] = variable;                                                                  \
  }

void ConnectionStyle::loadJson(QJsonObject const &json) {
  QJsonValue nodeStyleValues = json["ConnectionStyle"];

  QJsonObject obj = nodeStyleValues.toObject();

  CONNECTION_STYLE_READ_COLOR(obj, _constructionColor);
  CONNECTION_STYLE_READ_COLOR(obj, _normalColor);
  CONNECTION_STYLE_READ_COLOR(obj, _selectedColor);
  CONNECTION_STYLE_READ_COLOR(obj, _selectedHaloColor);
  CONNECTION_STYLE_READ_COLOR(obj, _hoveredColor);

  CONNECTION_STYLE_READ_FLOAT(obj, _lineWidth);
  CONNECTION_STYLE_READ_FLOAT(obj, _constructionLineWidth);
  CONNECTION_STYLE_READ_FLOAT(obj, _pointDiameter);

  CONNECTION_STYLE_READ_BOOL(obj, _useDataDefinedColors);
}

QJsonObject ConnectionStyle::toJson() const {
  QJsonObject obj;

  CONNECTION_STYLE_WRITE_COLOR(obj, _constructionColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, _normalColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, _selectedColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, _selectedHaloColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, _hoveredColor);

  CONNECTION_STYLE_WRITE_FLOAT(obj, _lineWidth);
  CONNECTION_STYLE_WRITE_FLOAT(obj, _constructionLineWidth);
  CONNECTION_STYLE_WRITE_FLOAT(obj, _pointDiameter);

  CONNECTION_STYLE_WRITE_BOOL(obj, _useDataDefinedColors);

  QJsonObject root;
  root["ConnectionStyle"] = obj;

  return root;
}

QColor ConnectionStyle::getNormalColor(QString typeId) const {
  std::size_t hash = qHash(typeId);

  std::size_t const hue_range = 0xFF;

  std::mt19937 gen(static_cast<unsigned int>(hash));
  std::uniform_int_distribution<int> distrib(0, hue_range);

  int hue = distrib(gen);
  int sat = 120 + hash % 129;

  return QColor::fromHsl(hue, sat, 160);
}

bool ConnectionStyle::operator==(const ConnectionStyle &other) {
  return _constructionColor == other._constructionColor && _normalColor == other._normalColor &&
         _selectedColor == other._selectedColor && _selectedHaloColor == other._selectedHaloColor &&
         _hoveredColor == other._hoveredColor && _lineWidth == other._lineWidth &&
         _constructionLineWidth == other._constructionLineWidth &&
         _pointDiameter == other._pointDiameter &&
         _useDataDefinedColors == other._useDataDefinedColors;
}
