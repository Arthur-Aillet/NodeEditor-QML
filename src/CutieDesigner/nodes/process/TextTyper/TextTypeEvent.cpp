#include "TextTypeEvent.hpp"

TextTypeEvent::TextTypeEvent() {}
TextTypeEvent::TextTypeEvent(const QString &name) : _value(variantFromName(name)) {}
TextTypeEvent::TextTypeEvent(VariantType &&variant) : _value(variant) {}

QString TextTypeEvent::name() const {
  auto nameVisitor = overload{[](const Wait &w) { return QString("Wait"); },
                              [](const Erase &e) { return QString("Erase"); },
                              [](const Replace &r) { return QString("Replace"); },
                              [](const Insert &i) { return QString("Insert"); }};
  return std::visit(nameVisitor, _value);
}

QJsonObject TextTypeEvent::save() const {
  QString eventName = name();
  QJsonObject object = std::visit(
      [eventName](auto &&v) {
        QJsonObject obj = v.save();
        obj["name"] = eventName;
        return obj;
      },
      _value);
  return object;
};

void TextTypeEvent::load(QJsonObject const &json) {
  QJsonValue name = json["name"];

  if (name.isUndefined())
    return;
  _value = variantFromName(name.toString());

  std::visit([json](auto &&v) { return v.load(json); }, _value);
};

TextTypeEvent TextTypeEvent::fromName(const QString &name) {
  return TextTypeEvent(variantFromName(name));
}

TextTypeEvent::VariantType &TextTypeEvent::value() { return _value; }

QVariant TextTypeEvent::variant() const {
  return std::visit([](auto &&v) { return QVariant::fromValue(v); }, _value);
}

TextTypeEvent::VariantType TextTypeEvent::variantFromName(const QString &name) {
  if (name == "Wait") {
    return VariantType(Wait{1});
  } else if (name == "Erase") {
    return VariantType(Erase());
  } else if (name == "Replace") {
    return VariantType(Replace());
  } else if (name == "Insert") {
    return VariantType(Insert());
  } else {
    qCritical() << "Invalid event tried to be created: " << name;
    return VariantType(Wait{0});
  }
}
