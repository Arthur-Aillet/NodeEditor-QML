#pragma once

#include "ConnectionStyle.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"
#include <QObject>
#include <qjsengine.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>

class StyleCollection : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  static StyleCollection &instance() {
    static StyleCollection instance;
    return instance;
  }

  static StyleCollection *create(QQmlEngine *qmlEngine, QJSEngine *) {
    auto instance = &StyleCollection::instance();
    QJSEngine::setObjectOwnership(instance, QJSEngine::ObjectOwnership::CppOwnership);
    return instance;
  }

  Q_PROPERTY(NodeStyle node MEMBER _nodeStyle NOTIFY nodeStyleChanged);
  Q_PROPERTY(ConnectionStyle connection MEMBER _connectionStyle NOTIFY connectionStyleChanged);
  Q_PROPERTY(
      GraphicsViewStyle graphicsView MEMBER _graphicsViewStyle NOTIFY graphicsViewStyleChanged);

  static void setNodeStyle(NodeStyle nodeStyle);
  static void setConnectionStyle(ConnectionStyle connectionStyle);
  static void setGraphicsViewStyle(GraphicsViewStyle graphicsViewStyle);

  static NodeStyle const &getNodeStyle();
  static ConnectionStyle const &getConnectionStyle();
  static GraphicsViewStyle const &getGraphicsViewStyle();

  signals:
  void nodeStyleChanged();
  void connectionStyleChanged();
  void graphicsViewStyleChanged();

  protected:
  StyleCollection(QObject *parent = nullptr);

  NodeStyle _nodeStyle;
  ConnectionStyle _connectionStyle;
  GraphicsViewStyle _graphicsViewStyle;
};
