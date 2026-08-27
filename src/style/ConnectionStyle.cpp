#include "ConnectionStyle.hpp"

#include <QVector3D>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>

#include <random>

using namespace NodeEditor;

// inline void initResources() { Q_INIT_RESOURCE(resources); }

ConnectionStyle::ConnectionStyle() {
  // Explicit resources inialization for preventing the static initialization
  // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
  // initResources();

  // This configuration is stored inside the compiled unit and is loaded statically
  loadJsonFile(":/NodeEditor/DefaultStyle.json");
}

ConnectionStyle::ConnectionStyle(QPalette const &palette) { loadPalette(palette); }

ConnectionStyle::ConnectionStyle(QString jsonText) {
  loadJsonFile(":/NodeEditor/DefaultStyle.json");
  loadJsonText(jsonText);
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

  CONNECTION_STYLE_READ_COLOR(obj, ConstructionColor);
  CONNECTION_STYLE_READ_COLOR(obj, NormalColor);
  CONNECTION_STYLE_READ_COLOR(obj, SelectedColor);
  CONNECTION_STYLE_READ_COLOR(obj, SelectedHaloColor);
  CONNECTION_STYLE_READ_COLOR(obj, HoveredColor);

  CONNECTION_STYLE_READ_FLOAT(obj, LineWidth);
  CONNECTION_STYLE_READ_FLOAT(obj, ConstructionLineWidth);
  CONNECTION_STYLE_READ_FLOAT(obj, PointDiameter);

  CONNECTION_STYLE_READ_BOOL(obj, UseDataDefinedColors);
}

QJsonObject ConnectionStyle::toJson() const {
  QJsonObject obj;

  CONNECTION_STYLE_WRITE_COLOR(obj, ConstructionColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, NormalColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, SelectedColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, SelectedHaloColor);
  CONNECTION_STYLE_WRITE_COLOR(obj, HoveredColor);

  CONNECTION_STYLE_WRITE_FLOAT(obj, LineWidth);
  CONNECTION_STYLE_WRITE_FLOAT(obj, ConstructionLineWidth);
  CONNECTION_STYLE_WRITE_FLOAT(obj, PointDiameter);

  CONNECTION_STYLE_WRITE_BOOL(obj, UseDataDefinedColors);

  QJsonObject root;
  root["ConnectionStyle"] = obj;

  return root;
}

void ConnectionStyle::loadPalette(QPalette const &palette) {
  loadJsonFile(":/NodeEditor/DefaultStyle.json");
  SelectedColor = palette.text().color();
  SelectedHaloColor = palette.highlight().color();
  ConstructionColor = palette.text().color().darker();
  NormalColor = palette.highlight().color();
  HoveredColor = palette.text().color();
}

QColor ConnectionStyle::constructionColor() const { return ConstructionColor; };
QColor ConnectionStyle::normalColor() const { return NormalColor; };

QColor ConnectionStyle::typeColor(QString typeId) const {
  std::size_t hash = qHash(typeId);

  std::size_t const hue_range = 0xFF;

  const std::size_t baseless_aesthetic_offset = 30;

  std::mt19937 gen(static_cast<unsigned int>(hash + baseless_aesthetic_offset));
  std::uniform_int_distribution<int> distrib(0, hue_range);

  int hue = distrib(gen);
  int sat = 120 + hash % 129;

  return QColor::fromHsl(hue, sat, 160);
}

QColor ConnectionStyle::selectedColor() const { return SelectedColor; };
QColor ConnectionStyle::selectedHaloColor() const { return SelectedHaloColor; };
QColor ConnectionStyle::hoveredColor() const { return HoveredColor; };

float ConnectionStyle::lineWidth() const { return LineWidth; };
float ConnectionStyle::constructionLineWidth() const { return ConstructionLineWidth; };
float ConnectionStyle::pointDiameter() const { return PointDiameter; };

bool ConnectionStyle::useDataDefinedColors() const { return UseDataDefinedColors; };

bool ConnectionStyle::operator==(const ConnectionStyle &other) const {
  return ConstructionColor == other.ConstructionColor && NormalColor == other.NormalColor &&
         SelectedColor == other.SelectedColor && SelectedHaloColor == other.SelectedHaloColor &&
         HoveredColor == other.HoveredColor && LineWidth == other.LineWidth &&
         ConstructionLineWidth == other.ConstructionLineWidth &&
         PointDiameter == other.PointDiameter && UseDataDefinedColors == other.UseDataDefinedColors;
}

//
// Copyright (c) 2020 Björn Ottosson
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// c.f. https://bottosson.github.io/posts/oklab/
//

static QVector3D toOklab(const QColor color) {
  const float l =
      0.4122214708f * color.redF() + 0.5363325363f * color.greenF() + 0.0514459929f * color.blueF();
  const float m =
      0.2119034982f * color.redF() + 0.6806995451f * color.greenF() + 0.1073969566f * color.blueF();
  const float s =
      0.0883024619f * color.redF() + 0.2817188376f * color.greenF() + 0.6299787005f * color.blueF();

  const float l_ = std::cbrtf(l);
  const float m_ = std::cbrtf(m);
  const float s_ = std::cbrtf(s);

  return {
      0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
      1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
      0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
  };
}

const QColor fromOklab(const QVector3D &oklab) {
  const float l_ = oklab.x() + 0.3963377774f * oklab.y() + 0.2158037573f * oklab.z();
  const float m_ = oklab.x() - 0.1055613458f * oklab.y() - 0.0638541728f * oklab.z();
  const float s_ = oklab.x() - 0.0894841775f * oklab.y() - 1.2914855480f * oklab.z();

  const float l = l_ * l_ * l_;
  const float m = m_ * m_ * m_;
  const float s = s_ * s_ * s_;

  return QColor::fromRgbF(+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
                          -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
                          -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s);
}

QColor ConnectionStyle::lerpOklabColors(const QColor &first, const QColor &second,
                                        const float amount) const {
  const auto oklabFirst = toOklab(first);
  const auto oklabSecond = toOklab(second);
  const auto lerped = oklabFirst + amount * (oklabSecond - oklabFirst);
  return fromOklab(lerped);
}
