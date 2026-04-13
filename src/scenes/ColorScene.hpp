#pragma once

#include "src/Scene.hpp"

class ColorScene : public Scene {
  Q_OBJECT

  public:
  ColorScene(QObject *parent) : Scene(parent) {}

  int m_value = 0;
  Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged);

  void valueChanged() { qDebug() << "New value: " << m_value; }
};
