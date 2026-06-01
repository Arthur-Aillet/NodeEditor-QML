#include "ObjectLoader.hpp"
#include <memory>
#include <qlogging.h>
#include <qquickitem.h>

ObjectLoader::ObjectLoader(QObject *parent) : QObject(parent) {}
ObjectLoader::~ObjectLoader() { _surface->deleteLater(); }

void ObjectLoader::connectFinalNode(ObjectDisplayDataModel *node) {
  QObject::connect(node, SIGNAL(componentSet(ObjectData *)), this,
                   SLOT(componentSet(ObjectData *)));

  QObject::connect(node, SIGNAL(componentRemoved()), this, SLOT(componentRemoved()));
}

void ObjectLoader::componentSet(ObjectData *object) {
  if (object->component->isNull()) {
    return;
  }
  _surface = qobject_cast<QQuickItem *>(
      object->component->createWithInitialProperties(object->initialProps));
  _surface->setParentItem(static_cast<QQuickItem *>(this->parent()));
  QJSEngine::setObjectOwnership(_surface, QJSEngine::CppOwnership);
  emit object->componentLoaded(_surface);
}

void ObjectLoader::componentRemoved() { _surface->deleteLater(); }