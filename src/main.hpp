#pragma once

#include <QProperty>
#include <QThread>
#include <memory>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qproperty.h>
#include <qqmlpropertyvaluesource.h>
#include <qthread.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include <utility>
#include <vector>

class Scene : public QObject {
  Q_OBJECT

  public:
  QProperty<int> value;

  virtual void display_value() = 0;

  template <typename T>
  void register_driver(std::string name, T val) {
    QObject::setProperty(name.c_str(), QVariant(val));
  }
};

class RedScene : public Scene {
  public:
  RedScene() = default;
  void display_value() override { qDebug() << "Value: " << value; }
};

constexpr int magic = 4;

class IncrementalObserver : public QThread {
  Q_OBJECT

  public:
  explicit IncrementalObserver(QObject *parent = nullptr) : QThread(parent) {}

  QProperty<int> intensity = QProperty<int>(0);

  protected:
  void run() override {
    while (true) {
      intensity.setValue(intensity + 1);
      emit setIntensity(intensity);
      qDebug() << "IncrementalObserver: " << intensity;
      this->sleep(2);
    }
  }

  signals:
  void setIntensity(int new_intensity);
};

class SceneManager : public QObject {
  Q_OBJECT
  std::vector<std::unique_ptr<Scene>> scenes;
  std::vector<std::unique_ptr<IncrementalObserver>> observers;

  public slots:
  void getIntensity(int intensity) { qDebug() << "Recived: " << intensity; }

  public:
  SceneManager() {
    observers.push_back(std::make_unique<IncrementalObserver>());
    observers[0]->start();
    QObject::connect(&*observers[0], SIGNAL(setIntensity(int)), this, SLOT(getIntensity(int)));

    scenes.push_back(std::make_unique<RedScene>());
    scenes[0]->display_value();

    scenes[0]->value.setBinding([&] { return observers[0]->property("intensity").value<int>(); });
    scenes[0]->display_value();
  }
};
