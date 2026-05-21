#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

struct Wait {
  Q_GADGET
  QML_VALUE_TYPE(wait)
  public:
  Q_PROPERTY(uint delay MEMBER delay)

  uint delay;
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
  Q_INVOKABLE void print();

  enum class EventRoles { Name = Qt::UserRole + 1, Value };
  QList<TypeEvent> events;

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(EventRoles::Name), "name"},
            {static_cast<int>(EventRoles::Value), "value"}};
  }
};
