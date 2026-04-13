#pragma once

#include <QProperty>
#include <QThread>
#include <memory>
#include <qdebug.h>
#include <qmetaobject.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qproperty.h>
#include <qqmlpropertyvaluesource.h>
#include <qthread.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include <string>
#include <unordered_map>
#include <vector>

class Scene : public QObject {
  Q_OBJECT

  public:
  int m_value = 0;
  Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged);

  virtual void display_value() = 0;
  void valueChanged() { qDebug() << "New value: " << m_value; }
  Scene(QObject *parent) : QObject(parent) {}
};

class RedScene : public Scene {
  public:
  RedScene(QObject *parent) : Scene(parent) {}
  void display_value() override { qDebug() << "Value: " << m_value; }
};

constexpr int magic = 4;

class IncrementalObserver : public QThread {
  Q_OBJECT

  public:
  explicit IncrementalObserver(QObject *parent = nullptr) : QThread(parent) {}

  int intensity = 0;

  protected:
  void run() override {
    while (true) {
      intensity += 1;
      emit setDriverValue("intensity", intensity);
      qDebug() << "IncrementalObserver: " << intensity;
      this->sleep(2);
    }
  }

  signals:
  void setDriverValue(std::string name, QVariant value);
};

class SceneManager : public QObject {
  Q_OBJECT
  std::vector<std::unique_ptr<Scene>> scenes;
  std::vector<std::unique_ptr<IncrementalObserver>> observers;
  std::unordered_multimap<std::string, QMetaProperty> driver_mapping;

  public slots:
  void getDriverValue(std::string name, QVariant value) {
    auto range = driver_mapping.equal_range(name);
    for (auto it = range.first; it != range.second; ++it) {
      auto [observerKey, scene_property] = *it;
      auto a = scenes[0]->property(scene_property.name()).toInt();
      qDebug() << "YES!" << observerKey << "  " << scene_property.name() << " : " << value << "  "
               << a;
      scenes[0]->setProperty(scene_property.name(), value);
    }
  }

  public:
  SceneManager(QObject *parent) : QObject(parent) {
    observers.push_back(std::make_unique<IncrementalObserver>());
    observers[0]->start();

    scenes.push_back(std::make_unique<RedScene>(this));
    scenes[0]->display_value();

    for (auto &scene : scenes) {
      const QMetaObject *metaObject = scene->metaObject();

      for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        driver_mapping.insert(std::pair("intensity", metaObject->property(i)));
      }
    }

    QObject::connect(&*observers[0], SIGNAL(setDriverValue(std::string, QVariant)), this,
                     SLOT(getDriverValue(std::string, QVariant)));
  }
};
