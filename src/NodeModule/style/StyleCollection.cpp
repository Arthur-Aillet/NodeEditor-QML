#include "StyleCollection.hpp"
#include "NodeStyle.hpp"
#include <qdebug.h>
#include <qtmetamacros.h>

StyleCollection::StyleCollection(QObject *parent)
    : QObject(parent), _nodeStyle(NodeStyle::defaultStyle()) {}

void StyleCollection::setGraphicsViewStyle(GraphicsViewStyle graphicsViewStyle) {
  instance()._graphicsViewStyle = graphicsViewStyle;
  emit instance().graphicsViewStyleChanged();
}

void StyleCollection::setConnectionStyle(ConnectionStyle connectionStyle) {
  instance()._connectionStyle = connectionStyle;
  emit instance().connectionStyleChanged();
}

void StyleCollection::setNodeStyle(NodeStyle nodeStyle) {
  instance()._nodeStyle = nodeStyle;
  emit instance().nodeStyleChanged();
}

NodeStyle const &StyleCollection::getNodeStyle() { return instance()._nodeStyle; }

ConnectionStyle const &StyleCollection::getConnectionStyle() { return instance()._connectionStyle; }

GraphicsViewStyle const &StyleCollection::getGraphicsViewStyle() {
  return instance()._graphicsViewStyle;
}