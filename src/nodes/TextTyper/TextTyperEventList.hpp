#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

struct TypeEvent {
  Q_GADGET
  QML_VALUE_TYPE(typeEvent)
  QML_STRUCTURED_VALUE

  public:
  enum State { Wait, Remove, Replace, Insert };
  Q_ENUM(State)

  State state;
};

class TextTyperEventList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  TextTyperEventList()
      : _events({TypeEvent{TypeEvent::State::Wait}, TypeEvent{TypeEvent::State::Remove},
                 TypeEvent{TypeEvent::State::Replace}, TypeEvent{TypeEvent::State::Insert}}),
        QAbstractListModel() {}

  explicit TextTyperEventList(const TextTyperEventList &other)
      : QAbstractListModel(other.parent()) {
    _events = other._events;
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    qDebug() << "n" << _events.count();
    return _events.count();
  }

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
