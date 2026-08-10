#pragma once

#include "ConnectionStyle.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>
#include <QtWidgets/QApplication>

class StyleCollection : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  static StyleCollection &instance();
  static StyleCollection *create(QQmlEngine *qmlEngine, QJSEngine *_jsEngine);

  Q_PROPERTY(NodeStyle node MEMBER _nodeStyle NOTIFY nodeStyleChanged);
  Q_PROPERTY(ConnectionStyle connection MEMBER _connectionStyle NOTIFY connectionStyleChanged);
  Q_PROPERTY(
      GraphicsViewStyle graphicsView MEMBER _graphicsViewStyle NOTIFY graphicsViewStyleChanged);

  static void setNodeStyle(NodeStyle nodeStyle);
  static void setConnectionStyle(ConnectionStyle connectionStyle);
  static void setGraphicsViewStyle(GraphicsViewStyle graphicsViewStyle);
  static void followApplicationPalette(bool enable);

  bool eventFilter(QObject *obj, QEvent *event) override;

  static NodeStyle const &getNodeStyle();
  static ConnectionStyle const &getConnectionStyle();
  static GraphicsViewStyle const &getGraphicsViewStyle();

  signals:
  void nodeStyleChanged();
  void connectionStyleChanged();
  void graphicsViewStyleChanged();

  protected:
  StyleCollection(QObject *parent = nullptr);

  bool _followApplicationPalette;
  NodeStyle _nodeStyle;
  ConnectionStyle _connectionStyle;
  GraphicsViewStyle _graphicsViewStyle;
};
