#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

struct Wait {
  Q_GADGET
  QML_VALUE_TYPE(wait)
  public:
  Q_PROPERTY(double delay MEMBER delay)

  double delay;
};

struct Erase {
  Q_GADGET
  QML_VALUE_TYPE(erase)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(uint amount MEMBER amount)

  uint pos;
  uint amount;
};

struct Replace {
  Q_GADGET
  QML_VALUE_TYPE(replace)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(QString text MEMBER text)

  uint pos;
  QString text;
};

struct Insert {
  Q_GADGET
  QML_VALUE_TYPE(insert)
  public:
  Q_PROPERTY(uint pos MEMBER pos)
  Q_PROPERTY(QString text MEMBER text)

  uint pos;
  QString text;
};

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

class TextTyperEventList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  using TypeEvent = std::variant<Wait, Erase, Replace, Insert>;

  TextTyperEventList() = default;

  explicit TextTyperEventList(const TextTyperEventList &other);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Q_INVOKABLE void addEvent(QString name);
  Q_INVOKABLE void removeEvent(int index);
  Q_INVOKABLE void editValue(int index, QString attribute, QVariant value);
  Q_INVOKABLE void print();
  QString variantName(TypeEvent const variant) const;
  TypeEvent eventFromName(QString const name) const;

  enum class EventRoles { Name = Qt::UserRole + 1, Value };
  QList<TypeEvent> events;

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(EventRoles::Name), "name"},
            {static_cast<int>(EventRoles::Value), "value"}};
  }
};
