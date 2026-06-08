#pragma once

#include "SurfaceData.hpp"
#include <QtCore/QObject>
#include <qdebug.h>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qtmetamacros.h>

class SurfaceLoader : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      SurfaceData *surfaceData READ surfaceData WRITE setSurfaceData NOTIFY surfaceDataChanged)
  Q_PROPERTY(QQuickItem *surface MEMBER _surface NOTIFY surfaceChanged)

  public:
  SurfaceLoader(QObject *parent = nullptr);
  SurfaceData *surfaceData() { return _surfaceData; }

  signals:
  void surfaceDataChanged();
  void surfaceChanged();

  private:
  SurfaceData *_surfaceData = nullptr;
  QQuickItem *_surface = nullptr;
  void createComponent(SurfaceData *surface);
  void removeComponent();

  public slots:
  void setSurfaceData(SurfaceData *surfaceData);
};