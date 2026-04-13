#pragma once

#include <QDebug>
#include <QObject>

class Scene : public QObject {
  Q_OBJECT

  public:
  int m_value = 0;
  Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged);

  virtual void display_value() = 0;
  void valueChanged() { qDebug() << "New value: " << m_value; }
  Scene(QObject *parent) : QObject(parent) {}
};
