#include "SurfaceLoader.hpp"

#include <qdebug.h>
#include <qjsengine.h>
#include <qlogging.h>
#include <qobject.h>
#include <qquickitem.h>
#include <qvariant.h>

SurfaceLoader::SurfaceLoader(QQuickItem *parent) : QQuickItem(parent) {}

void SurfaceLoader::createComponent(SurfaceData *surfaceDescription) {
  if (surfaceDescription->component()->isNull())
    return;

  if (surfaceDescription->component()->isError()) {
    qCritical() << "AType letter componenent:" << surfaceDescription->component()->errorString();
    return;
  }
  if (!surfaceDescription->component()->isReady()) {
    qCritical() << "AType letter componenent: Not Ready to be created";
    return;
  }

  QQuickItem *nextItem = nullptr;
  bool loaderReached = false;

  // if (parentItem() != nullptr)
  //   for (auto sibling : parentItem()->children()) {
  //     if (sibling == this) {
  //       loaderReached = true;
  //     } else if (sibling->isQuickItemType() && loaderReached) {
  //       nextItem = qobject_cast<QQuickItem *>(sibling);
  //       break;
  //     }
  //   }
  _surface =
      qobject_cast<QQuickItem *>(surfaceDescription->component()->createWithInitialProperties(
          surfaceDescription->initialProps()));
  _surface->setParentItem(static_cast<QQuickItem *>(this));
  if (_anchored) {
    qvariant_cast<QObject *>(this->property("anchors"))
        ->setProperty("fill", QVariant::fromValue(parentItem()));
  }
  //_surface->stackAfter(this);
  // if (nextItem != nullptr) {
  // }
  QJSEngine::setObjectOwnership(_surface, QJSEngine::CppOwnership);
  emit surfaceDescription->componentLoaded(_surface);
  emit surfaceChanged();
}

void SurfaceLoader::removeComponent() {
  if (_surface == nullptr)
    return;

  // Delete _surface with delete instead of deleteLater()
  // to delete the QQuickItem as soon as possible, before the nodes they depend on are destroyed
  delete _surface;
  _surface = nullptr;
}

void SurfaceLoader::setSurfaceData(SurfaceData *surfaceData) {
  if (surfaceData == _surfaceData)
    return;

  _surfaceData = surfaceData;
  if (surfaceData == nullptr) {
    removeComponent();
  } else {
    createComponent(_surfaceData);
  }
  emit surfaceDataChanged();
}