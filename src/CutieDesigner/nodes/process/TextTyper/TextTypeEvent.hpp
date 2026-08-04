#pragma once

#include "Serializable.hpp"

#include <QQmlEngine>

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

struct Wait : public Serializable {
  Q_GADGET
  QML_VALUE_TYPE(wait)
  public:
  Q_PROPERTY(double delay MEMBER delay)

  public:
  double delay;

  Wait(double delay = 0.0) : delay(delay) {}

  QJsonObject save() const override { return {{"delay", delay}}; }
  void load(QJsonObject const &json) override {
    QJsonValue delayValue = json["delay"];

    if (!delayValue.isUndefined()) {
      delay = delayValue.toDouble();
    }
  }
};

struct Erase : public Serializable {
  Q_GADGET
  QML_VALUE_TYPE(erase)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(uint amount MEMBER amount)

  Erase(uint pos = 0, uint amount = 0) : pos(pos), amount(amount) {}

  uint pos;
  uint amount;

  QJsonObject save() const override { return {{"pos", (int)pos}, {"amount", (int)amount}}; }
  void load(QJsonObject const &json) override {
    QJsonValue posValue = json["pos"];
    QJsonValue amountValue = json["amount"];

    if (!posValue.isUndefined()) {
      pos = posValue.toInt();
    }
    if (!amountValue.isUndefined()) {
      amount = amountValue.toInt();
    }
  }
};

struct Replace : public Serializable {
  Q_GADGET
  QML_VALUE_TYPE(replace)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(QString text MEMBER text)

  Replace(uint pos = 0, QString text = "") : pos(pos), text(text) {}

  QJsonObject save() const override { return {{"pos", (int)pos}, {"text", text}}; }
  void load(QJsonObject const &json) override {
    QJsonValue posValue = json["pos"];
    QJsonValue textValue = json["text"];

    if (!posValue.isUndefined()) {
      pos = posValue.toInt();
    }
    if (!textValue.isUndefined()) {
      text = textValue.toString();
    }
  }

  uint pos;
  QString text;
};

struct Insert : public Serializable {
  Q_GADGET
  QML_VALUE_TYPE(insert)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(QString text MEMBER text)

  Insert(uint pos = 0, QString text = "") : pos(pos), text(text) {}
  QJsonObject save() const override { return {{"pos", (int)pos}, {"text", text}}; }
  void load(QJsonObject const &json) override {
    QJsonValue posValue = json["pos"];
    QJsonValue textValue = json["text"];

    if (!posValue.isUndefined()) {
      pos = posValue.toInt();
    }
    if (!textValue.isUndefined()) {
      text = textValue.toString();
    }
  }

  uint pos;
  QString text;
};

class TextTypeEvent : public Serializable {
  public:
  using VariantType = std::variant<Wait, Erase, Replace, Insert>;

  TextTypeEvent();
  TextTypeEvent(const QString &name);
  TextTypeEvent(VariantType &&variant);

  QJsonObject save() const override;
  void load(QJsonObject const &) override;

  QString name() const;
  static TextTypeEvent fromName(const QString &name);
  VariantType &value();
  QVariant variant() const;

  protected:
  static VariantType variantFromName(const QString &name);

  VariantType _value;
};