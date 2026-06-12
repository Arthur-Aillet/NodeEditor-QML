#pragma once

#include "SurfaceData.hpp"

#include <QtCore/QObject>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qtmetamacros.h>

class SurfaceLoader : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      SurfaceData *surfaceData READ surfaceData WRITE setSurfaceData NOTIFY surfaceDataChanged)
  Q_PROPERTY(QQuickItem *surface MEMBER _surface NOTIFY surfaceChanged)
  Q_PROPERTY(bool anchored MEMBER _anchored NOTIFY surfaceChanged)

  public:
  SurfaceLoader(QQuickItem *parent = nullptr);
  SurfaceData *surfaceData() { return _surfaceData; }
  ~SurfaceLoader() {
    if (_surface != nullptr)
      delete _surface;
  };

  signals:
  void surfaceDataChanged();
  void surfaceChanged();
  void anchoredChanged();

  private:
  SurfaceData *_surfaceData = nullptr;
  QQuickItem *_surface = nullptr;
  bool _anchored = true;
  void createComponent(SurfaceData *surface);
  void removeComponent();

  public slots:
  void setSurfaceData(SurfaceData *surfaceData);
};