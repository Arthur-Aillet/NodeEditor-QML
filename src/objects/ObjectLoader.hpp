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

  void connectFinalNode(ObjectDisplayDataModel *node);

  private:
  std::shared_ptr<QQuickItem> _surface{nullptr};

  public slots:
  void componentSet(ObjectData *object);
  void componentRemoved();
};