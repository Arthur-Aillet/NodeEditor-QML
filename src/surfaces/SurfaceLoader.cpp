#include "SurfaceLoader.hpp"

#include <qjsengine.h>
#include <qlogging.h>
#include <qquickitem.h>

SurfaceLoader::SurfaceLoader(QObject *parent) : QObject(parent) {}

void SurfaceLoader::connectFinalNode(SurfaceDisplayDataModel *node) {
  QObject::connect(node, SIGNAL(componentSet(SurfaceData *)), this,
                   SLOT(componentSet(SurfaceData *)));

  QObject::connect(node, SIGNAL(componentRemoved()), this, SLOT(componentRemoved()));
}

void SurfaceLoader::componentSet(SurfaceData *object) {
  if (object->component->isNull()) {
    return;
  }
  _surface = qobject_cast<QQuickItem *>(
      object->component->createWithInitialProperties(object->initialProps));
  _surface->setParentItem(static_cast<QQuickItem *>(this->parent()));
  QJSEngine::setObjectOwnership(_surface, QJSEngine::CppOwnership);
  emit object->componentLoaded(_surface);
}

void SurfaceLoader::componentRemoved() { _surface->deleteLater(); }