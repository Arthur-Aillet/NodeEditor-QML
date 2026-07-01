#include "StyleCollection.hpp"
#include "ConnectionStyle.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"

StyleCollection::StyleCollection(QObject *parent)
    : QObject(parent), _followApplicationPalette(true), _graphicsViewStyle(QApplication::palette()),
      _connectionStyle(QApplication::palette()), _nodeStyle(QApplication::palette()) {}

void StyleCollection::followApplicationPalette(bool enable) {
  auto &instance = StyleCollection::instance();

  if (enable == instance._followApplicationPalette)
    return;

  instance._followApplicationPalette = enable;
  if (enable) {
    auto palette = QApplication::palette();
    instance.setConnectionStyle(ConnectionStyle(palette));
    instance.setGraphicsViewStyle(GraphicsViewStyle(palette));
    instance.setNodeStyle(NodeStyle(palette));
  } else {
    instance.setConnectionStyle(ConnectionStyle());
    instance.setGraphicsViewStyle(GraphicsViewStyle());
    instance.setNodeStyle(NodeStyle::defaultStyle());
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

StyleCollection &StyleCollection::instance() {
  static StyleCollection instance;
  return instance;
}

StyleCollection *StyleCollection::create(QQmlEngine *qmlEngine, QJSEngine *_jsEngine) {
  auto instance = &StyleCollection::instance();
  QJSEngine::setObjectOwnership(instance, QJSEngine::ObjectOwnership::CppOwnership);
  QApplication::instance()->installEventFilter(instance);
  return instance;
}

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