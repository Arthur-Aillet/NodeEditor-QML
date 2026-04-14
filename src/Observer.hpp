#pragma once

#include <QDebug>
#include <QThread>
#include <QVariant>
#include <qproperty.h>
#include <string>
#include <vector>

#define REGISTER(a) registerProperty(#a, a) // NOLINT

class Observer : public QThread {
  Q_OBJECT

  using onChangeFunctor = std::function<void()>;

  std::vector<QPropertyChangeHandler<onChangeFunctor>> handlers{};

  public:
  explicit Observer(QObject *parent = nullptr) : QThread(parent) {}

  template <typename T>
  void registerProperty(std::string name, QProperty<T> &property) {
    QProperty<T> *ptr = &property;

    handlers.push_back(property.onValueChanged(
        onChangeFunctor([this, ptr, name]() { emit setDriverValue(name, ptr->value()); })));
  }
  virtual void init() = 0;

  protected:
  void run() override = 0;

  signals:
  void setDriverValue(std::string name, QVariant value);
};
