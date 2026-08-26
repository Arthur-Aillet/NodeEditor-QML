#include "StyleCollection.hpp"
#include "ConnectionStyle.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"

using namespace NodeEditor;

StyleCollection::StyleCollection(QObject *parent)
    : QObject(parent), _followApplicationPalette(true), _graphicsViewStyle(QApplication::palette()),
      _connectionStyle(QApplication::palette()), _nodeStyle(QApplication::palette()) {
  QApplication::instance()->installEventFilter(this);
}

void StyleCollection::followApplicationPalette(bool enable) {
  if (enable == _followApplicationPalette)
    return;

  _followApplicationPalette = enable;
  if (enable) {
    auto palette = QApplication::palette();
    setConnectionStyle(ConnectionStyle(palette));
    setGraphicsViewStyle(GraphicsViewStyle(palette));
    setNodeStyle(NodeStyle(palette));
  } else {
    setConnectionStyle(ConnectionStyle());
    setGraphicsViewStyle(GraphicsViewStyle());
    setNodeStyle(NodeStyle::defaultStyle());
  }
}

bool StyleCollection::eventFilter(QObject *obj, QEvent *event) {
  if (_followApplicationPalette && event->type() == QEvent::ApplicationPaletteChange) {
    auto palette = QApplication::palette();
    setConnectionStyle(ConnectionStyle(palette));
    setGraphicsViewStyle(GraphicsViewStyle(palette));
    setNodeStyle(NodeStyle(palette));

    return true;
  } else {
    return QObject::eventFilter(obj, event);
  }
}

void StyleCollection::setGraphicsViewStyle(GraphicsViewStyle graphicsViewStyle) {
  _graphicsViewStyle = graphicsViewStyle;
  emit graphicsViewStyleChanged();
}

void StyleCollection::setConnectionStyle(ConnectionStyle connectionStyle) {
  _connectionStyle = connectionStyle;
  emit connectionStyleChanged();
}

void StyleCollection::setNodeStyle(NodeStyle nodeStyle) {
  _nodeStyle = nodeStyle;
  emit nodeStyleChanged();
}

NodeStyle const &StyleCollection::nodeStyle() { return _nodeStyle; }
ConnectionStyle const &StyleCollection::connectionStyle() { return _connectionStyle; }
GraphicsViewStyle const &StyleCollection::graphicsViewStyle() { return _graphicsViewStyle; }
