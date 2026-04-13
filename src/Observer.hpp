#pragma once

#include <QDebug>
#include <QThread>
#include <QVariant>
#include <string>

#define UPDATE_VALUE(a) setDriverValue(#a, a) // NOLINT

class Observer : public QThread {
  Q_OBJECT

  public:
  explicit Observer(QObject *parent = nullptr) : QThread(parent) {}

  protected:
  void run() override = 0;

  signals:
  void setDriverValue(std::string name, QVariant value);
};
