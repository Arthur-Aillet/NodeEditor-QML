#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

struct TypeEvent {
  Q_GADGET
  QML_VALUE_TYPE(typeEvent)
  QML_STRUCTURED_VALUE

  public:
  enum State { Wait, Erase, Replace, Insert };
  Q_ENUM(State)

  State state;
};

class TextTyperEventList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  TextTyperEventList() = default;

  explicit TextTyperEventList(const TextTyperEventList &other)
      : QAbstractListModel(other.parent()) {
    _events = other._events;
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const override { return _events.count(); }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
    if (!checkIndex(index))
      return QVariant();

    const auto &rowData = _events.at(index.row());
    switch (static_cast<EventRoles>(role)) {
    case EventRoles::State:
      return rowData.state;
      break;
    case EventRoles::Name:
      return QMetaEnum::fromType<TypeEvent::State>().valueToKey(rowData.state);
      break;
    }

    return QVariant();
  }

  Q_INVOKABLE void addEvent(QString name) {
    auto enumVal = QMetaEnum::fromType<TypeEvent::State>().keyToValue(name.toLatin1().data());
    beginInsertRows({}, rowCount(), rowCount());
    _events.append(TypeEvent{TypeEvent::State(enumVal)});
    endInsertRows();
  }

  Q_INVOKABLE void removeEvent(int index) {
    beginRemoveRows(QModelIndex(), index, index);
    _events.removeAt(index);
    endRemoveRows();
  }

  Q_INVOKABLE void print() {
    qDebug() << "\nPrinting from TextTyperEventList::print()";
    for (const auto &event : _events) {
      qDebug() << event.state << " state";
    }
  }

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(EventRoles::State), "state"},
            {static_cast<int>(EventRoles::Name), "name"}};
  }

  private:
  enum class EventRoles { State = Qt::UserRole + 1, Name };

  QList<TypeEvent> _events;
};
