#include "AbstractContext.hpp"

using namespace NodeEditor;

AbstractContext::AbstractContext(AbstractGraph *graphModel, QQmlEngine *engine)
    : _graphModel(graphModel), QObject(engine) {
  _nodePainter = std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultNodePainter");
  _connectionPainter =
      std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultConnectionPainter");
  _styleCollection = new StyleCollection(this);
};

StyleCollection *AbstractContext::styleCollection() { return _styleCollection; }
void AbstractContext::setStyleCollection(StyleCollection *styleCollection) {
  _styleCollection = styleCollection;
  _styleCollection->setParent(this);
  emit styleCollectionChanged();
}

QQmlComponent *AbstractContext::nodePainter() { return _nodePainter.get(); }
void AbstractContext::setNodePainter(std::unique_ptr<QQmlComponent> nodePainter) {
  _nodePainter = std::move(nodePainter);
  emit nodePainterChanged();
}

QQmlComponent *AbstractContext::connectionPainter() { return _connectionPainter.get(); }
void AbstractContext::setConnectionPainter(std::unique_ptr<QQmlComponent> connectionPainter) {
  _connectionPainter = std::move(connectionPainter);
  emit nodePainterChanged();
}

AbstractGraph *AbstractContext::graphModel() { return _graphModel; }
