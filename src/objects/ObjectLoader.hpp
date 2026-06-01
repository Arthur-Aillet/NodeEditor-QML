#pragma once

#include "ObjectDisplayDataModel.hpp"
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class ObjectLoader : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  ObjectLoader(QObject *parent = nullptr);
  ~ObjectLoader();

  void connectFinalNode(ObjectDisplayDataModel *node);

  private:
  QQuickItem *_surface;

  public slots:
  void componentSet(ObjectData *object);
  void componentRemoved();
};