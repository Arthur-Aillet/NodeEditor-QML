#include "TextTyperEventList.hpp"

TextTyperEventList::TextTyperEventList(const TextTyperEventList &other)
    : QAbstractListModel(other.parent()) {
  events = other.events;
}

int TextTyperEventList::rowCount(const QModelIndex &parent) const { return events.count(); }

static QString variantName(TextTyperEventList::TypeEvent variant) {
  switch (variant.index()) {
  case 0:
    return QString("Wait");
  case 1:
    return QString("Erase");
  case 2:
    return QString("Replace");
  case 3:
    return QString("Insert");
  default:
    return QString();
  }
}

static QVariant variantValue(TextTyperEventList::TypeEvent variant) {
  switch (variant.index()) {
  case 0:
    return QVariant::fromValue(std::get<0>(variant));
  case 1:
    return QVariant::fromValue(std::get<1>(variant));
  case 2:
    return QVariant::fromValue(std::get<2>(variant));
  case 3:
    return QVariant::fromValue(std::get<3>(variant));
  default:
    return QVariant();
  }
}

static TextTyperEventList::TypeEvent eventFromName(QString name) {
  if (name == "Wait") {
    return TextTyperEventList::TypeEvent(Wait{1});
  } else if (name == "Erase") {
    return TextTyperEventList::TypeEvent(Erase{0, 0});
  } else if (name == "Replace") {
    return TextTyperEventList::TypeEvent(Replace{0, ""});
  } else if (name == "Insert") {
    return TextTyperEventList::TypeEvent(Insert{0, ""});
  } else {
    qCritical() << "Invalid event tried to be created: " << name;
    return TextTyperEventList::TypeEvent(Wait{0});
  }
}

QVariant TextTyperEventList::data(const QModelIndex &index, int role) const {
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

void TextTyperEventList::addEvent(QString name) {
  beginInsertRows({}, rowCount(), rowCount());
  events.append(eventFromName(name));
  endInsertRows();
}

void TextTyperEventList::removeEvent(int index) {
  beginRemoveRows(QModelIndex(), index, index);
  events.removeAt(index);
  endRemoveRows();
}

void TextTyperEventList::print() {
  qDebug() << "\nPrinting from TextTyperEventList::print()";
  for (const auto &event : events) {
    qDebug() << variantName(event) << " state";
  }
}
