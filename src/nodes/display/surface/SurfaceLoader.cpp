#include "SurfaceLoader.hpp"

#include <qdebug.h>
#include <qjsengine.h>
#include <qlogging.h>
#include <qquickitem.h>

SurfaceLoader::SurfaceLoader(QObject *parent) : QObject(parent) {}

void SurfaceLoader::createComponent(SurfaceData *surfaceDescription) {
  if (surfaceDescription->component->isNull())
    return;

  if (surfaceDescription->component->isError()) {
    qCritical() << "AType letter componenent:" << surfaceDescription->component->errorString();
    return;
  }
  if (!surfaceDescription->component->isReady()) {
    qCritical() << "AType letter componenent: Not Ready to be created";
    return;
  }

  QQuickItem *nextItem = nullptr;
  bool loaderReached = false;

  for (auto sibling : parent()->children()) {
    if (sibling == this) {
      loaderReached = true;
    } else if (sibling->isQuickItemType() && loaderReached) {
      nextItem = qobject_cast<QQuickItem *>(sibling);
      break;
    }
  }
  _surface = qobject_cast<QQuickItem *>(
      surfaceDescription->component->createWithInitialProperties(surfaceDescription->initialProps));
  _surface->setParentItem(static_cast<QQuickItem *>(parent()));
  if (nextItem != nullptr) {
    _surface->stackBefore(nextItem);
  }
  QJSEngine::setObjectOwnership(_surface, QJSEngine::CppOwnership);
  emit surfaceDescription->componentLoaded(_surface);
  emit surfaceChanged();
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