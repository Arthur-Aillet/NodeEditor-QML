#pragma once

#include <QDebug>
#include <QObject>

class Scene : public QObject {
  public:
  Scene(QObject *parent) : QObject(parent) {}
};
