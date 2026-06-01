#pragma once

#include "SurfaceDisplayNode.hpp"
#include <QtCore/QObject>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qtmetamacros.h>

class SurfaceLoader : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  SurfaceLoader(QObject *parent = nullptr);

  void connectFinalNode(SurfaceDisplayNode *node);

  private:
  QQuickItem *_surface;

  public slots:
  void componentSet(SurfaceData *surface);
  void componentRemoved();
};