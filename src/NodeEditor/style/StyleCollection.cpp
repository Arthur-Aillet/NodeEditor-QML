#include "StyleCollection.hpp"
#include "NodeStyle.hpp"

StyleCollection::StyleCollection(QObject *parent)
    : QObject(parent), _trackApplicationPalette(true), _graphicsViewStyle(QApplication::palette()),
      _connectionStyle(QApplication::palette()), _nodeStyle(QApplication::palette()) {}

bool StyleCollection::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ApplicationPaletteChange) {
    _graphicsViewStyle.loadPalette(QApplication::palette());
    _connectionStyle.loadPalette(QApplication::palette());
    _nodeStyle.loadPalette(QApplication::palette());
    emit graphicsViewStyleChanged();
    emit connectionStyleChanged();
    emit nodeStyleChanged();

    return true;
  } else {
    return QObject::eventFilter(obj, event);
  }
}

StyleCollection &StyleCollection::instance() {
  static bool init = false;
  static StyleCollection instance;
  if (!init) {
    QApplication::instance()->installEventFilter(&instance);
    init = true;
  }
  return instance;
}

StyleCollection *StyleCollection::create(QQmlEngine *qmlEngine, QJSEngine *_jsEngine) {
  auto instance = &StyleCollection::instance();
  QJSEngine::setObjectOwnership(instance, QJSEngine::ObjectOwnership::CppOwnership);
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