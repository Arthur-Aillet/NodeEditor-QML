#include "SurfaceLoader.hpp"

#include <qdebug.h>
#include <qjsengine.h>
#include <qlogging.h>
#include <qquickitem.h>

SurfaceLoader::SurfaceLoader(QObject *parent) : QObject(parent) {}

void SurfaceLoader::createComponent(SurfaceData *surfaceDescription) {
  if (surfaceDescription->component->isNull())
    return;

  _surface = qobject_cast<QQuickItem *>(
      surfaceDescription->component->createWithInitialProperties(surfaceDescription->initialProps));
  _surface->setParentItem(static_cast<QQuickItem *>(this->parent()));
  QJSEngine::setObjectOwnership(_surface, QJSEngine::CppOwnership);
  emit surfaceDescription->componentLoaded(_surface);
}

void SurfaceLoader::removeComponent() {
  if (_surface == nullptr)
    return;

  _surface->deleteLater();
}

void SurfaceLoader::setSurfaceData(SurfaceData *surfaceData) {
  if (surfaceData == _surfaceData)
    return;

  if (surfaceData == nullptr) {
    _surfaceData = surfaceData;
    removeComponent();
    emit surfaceDataChanged();
    return;
  }

  _surfaceData = surfaceData;
  createComponent(_surfaceData);
  emit surfaceDataChanged();
}