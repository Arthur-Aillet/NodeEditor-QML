#include "TextTyperEventList.hpp"

using TTEL = TextTyperEventList;

TTEL::TextTyperEventList(const TextTyperEventList &other) : QAbstractListModel(other.parent()) {
  events = other.events;
}

int TTEL::rowCount(const QModelIndex &parent) const { return events.count(); }

QString TTEL::variantName(const TTEL::TypeEvent variant) const {
  auto nameVisitor = overload{[](const Wait &w) { return QString("Wait"); },
                              [](const Erase &e) { return QString("Erase"); },
                              [](const Replace &r) { return QString("Replace"); },
                              [](const Insert &i) { return QString("Insert"); }};
  return std::visit(nameVisitor, variant);
}

static QVariant variantValue(TTEL::TypeEvent variant) {
  auto valueVisitor = overload{[](Wait w) { return QVariant::fromValue(w); },
                               [](Erase e) { return QVariant::fromValue(e); },
                               [](Replace r) { return QVariant::fromValue(r); },
                               [](Insert i) { return QVariant::fromValue(i); }};
  return std::visit(valueVisitor, variant);
}

TTEL::TypeEvent TTEL::eventFromName(const QString name) const {
  if (name == "Wait") {
    return TTEL::TypeEvent(Wait{1});
  } else if (name == "Erase") {
    return TTEL::TypeEvent(Erase{0, 0});
  } else if (name == "Replace") {
    return TTEL::TypeEvent(Replace{0, ""});
  } else if (name == "Insert") {
    return TTEL::TypeEvent(Insert{0, ""});
  } else {
    qCritical() << "Invalid event tried to be created: " << name;
    return TTEL::TypeEvent(Wait{0});
  }
}

QVariant TTEL::data(const QModelIndex &index, int role) const {
  if (!checkIndex(index))
    return QVariant();

  const auto &rowData = events.at(index.row());
  switch (static_cast<EventRoles>(role)) {
  case EventRoles::Value:
    return variantValue(rowData);
  case EventRoles::Name:
    return variantName(rowData);
  }

  return QVariant();
}

void TTEL::addEvent(QString name) {
  beginInsertRows({}, rowCount(), rowCount());
  events.append(eventFromName(name));
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
      w.delay = value.toUInt();
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
      events[index]);
  qDebug() << variantValue(events[index]);
}

void TTEL::print() {
  qDebug() << "\nPrinting from TextTyperEventList::print()";
  for (const auto &event : events) {
    qDebug() << variantName(event) << " state";
  }
}
