#include "TextTyperEventList.hpp"
#include "TextTypeEvent.hpp"
#include <QJsonArray>

using TTEL = TextTyperEventList;

TTEL::TextTyperEventList(const TextTyperEventList &other) : QAbstractListModel(other.parent()) {
  events = other.events;
}

int TTEL::rowCount(const QModelIndex &parent) const { return events.count(); }

QVariant TTEL::data(const QModelIndex &index, int role) const {
  if (!checkIndex(index))
    return QVariant();

  const auto &rowData = events.at(index.row());
  switch (static_cast<EventRoles>(role)) {
  case EventRoles::Value:
    return rowData.variant();
  case EventRoles::Name:
    return rowData.name();
  }

  return QVariant();
}

QJsonObject TTEL::save() const {
  QJsonArray eventArray;
  for (const auto &event : events) {
    eventArray.append(event.save());
  }
  return QJsonObject({{"eventList", QJsonValue(eventArray)}});
}

void TTEL::load(QJsonObject const &json) {
  QJsonValue valueList = json["eventList"];

  if (!valueList.isUndefined()) {
    QJsonArray eventArray = valueList.toArray();
    beginRemoveRows(QModelIndex(), 0, rowCount());
    events.clear();
    endRemoveRows();
    beginInsertRows({}, 0, eventArray.count());
    for (const auto &event : eventArray) {
      TextTypeEvent newEvent;
      newEvent.load(event.toObject());
      events.append(newEvent);
    }
    endInsertRows();
  }
}

void TTEL::addEvent(QString name) {
  beginInsertRows({}, rowCount(), rowCount());
  events.append(TextTypeEvent::fromName(name));
  endInsertRows();
}

void TTEL::removeEvent(int index) {
  beginRemoveRows(QModelIndex(), index, index);
  events.removeAt(index);
  endRemoveRows();
}

void TTEL::editValue(int index, QString attribute, QVariant value) {
  auto waitLambda = [attribute, value](Wait &w) {
    if (attribute == "delay") {
      w.delay = value.toDouble();
    }
  };

  auto eraseLambda = [attribute, value](Erase &e) {
    if (attribute == "amount")
      e.amount = value.toUInt();
    else if (attribute == "pos")
      e.pos = value.toUInt();
  };

  auto replaceLambda = [attribute, value](Replace &r) {
    if (attribute == "text")
      r.text = value.toString();
    else if (attribute == "pos")
      r.pos = value.toUInt();
  };

  auto insertLambda = [attribute, value](Insert &i) {
    if (attribute == "text")
      i.text = value.toString();
    else if (attribute == "pos")
      i.pos = value.toUInt();
  };

  std::visit(
      overload{
          waitLambda,
          eraseLambda,
          replaceLambda,
          insertLambda,
      },
      events[index].value());
}

void TTEL::print() {
  qDebug() << "\nPrinting from TextTyperEventList::print()";
  for (const auto &event : events) {
    qDebug() << event.name() << " state";
  }
}
