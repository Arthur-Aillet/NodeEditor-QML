#pragma once

#include "src/Scene.hpp"

class ColorScene : public Scene {
  public:
  ColorScene(QObject *parent) : Scene(parent) {}
  void display_value() override { qDebug() << "Value: " << m_value; }
};
