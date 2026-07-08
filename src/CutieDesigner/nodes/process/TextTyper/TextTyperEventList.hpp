#pragma once

#include "Serializable.hpp"
#include "TextTypeEvent.hpp"

#include <QAbstractListModel>
#include <QQmlEngine>

class TextTyperEventList : public QAbstractListModel, public Serializable {
  Q_OBJECT
  QML_ELEMENT

  public:
  TextTyperEventList() = default;

  explicit TextTyperEventList(const TextTyperEventList &other);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Q_INVOKABLE void addEvent(QString name);
  Q_INVOKABLE void removeEvent(int index);
  Q_INVOKABLE void editValue(int index, QString attribute, QVariant value);
  Q_INVOKABLE void print();

  QJsonObject save() const override;
  void load(QJsonObject const &) override;

  enum class EventRoles { Name = Qt::UserRole + 1, Value };
  QList<TextTypeEvent> events;

  protected:
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(EventRoles::Name), "name"},
            {static_cast<int>(EventRoles::Value), "value"}};
  }
};
