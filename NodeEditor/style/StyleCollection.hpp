#pragma once

#include "ConnectionStyle.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>
#include <QtWidgets/QApplication>

namespace NodeEditor {
class StyleCollection : public QObject {
  Q_OBJECT
  QML_ELEMENT

  public:
  StyleCollection(QObject *parent = nullptr);

  Q_PROPERTY(NodeEditor::NodeStyle node READ nodeStyle NOTIFY nodeStyleChanged);
  Q_PROPERTY(
      NodeEditor::ConnectionStyle connection READ connectionStyle NOTIFY connectionStyleChanged);
  Q_PROPERTY(NodeEditor::GraphicsViewStyle graphicsView READ graphicsViewStyle NOTIFY
                 graphicsViewStyleChanged);

  void setNodeStyle(NodeStyle nodeStyle);
  void setConnectionStyle(ConnectionStyle connectionStyle);
  void setGraphicsViewStyle(GraphicsViewStyle graphicsViewStyle);
  void followApplicationPalette(bool enable);

  NodeStyle const &nodeStyle();
  ConnectionStyle const &connectionStyle();
  GraphicsViewStyle const &graphicsViewStyle();

  signals:
  void nodeStyleChanged();
  void connectionStyleChanged();
  void graphicsViewStyleChanged();

  protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

  bool _followApplicationPalette;
  NodeStyle _nodeStyle;
  ConnectionStyle _connectionStyle;
  GraphicsViewStyle _graphicsViewStyle;
};
} // namespace NodeEditor