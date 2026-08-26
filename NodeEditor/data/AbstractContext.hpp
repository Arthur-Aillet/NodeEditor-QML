#pragma once

#include "AbstractGraphModel.hpp"
#include "StyleCollection.hpp"

#include <memory>

#include <QQmlComponent>
#include <QQmlEngine>
#include <QtQml>
#include <QtQmlIntegration>
#include <qqmlcomponent.h>

class AbstractContext : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(AbstractGraphModel *graphModel READ graphModel CONSTANT)
  Q_PROPERTY(QQmlComponent *nodePainter READ nodePainter NOTIFY nodePainterChanged)
  Q_PROPERTY(
      QQmlComponent *connectionPainter READ connectionPainter NOTIFY connectionPainterChanged)
  Q_PROPERTY(StyleCollection *styles READ styleCollection NOTIFY styleCollectionChanged)

  AbstractContext(AbstractGraphModel *graphModel, QQmlEngine *engine)
      : _graphModel(graphModel), QObject(engine) {
    _nodePainter = std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultNodePainter");
    _connectionPainter =
        std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultConnectionPainter");
    _styleCollection = new StyleCollection(this);
  };

  StyleCollection *styleCollection() { return _styleCollection; }
  void setStyleCollection(StyleCollection *styleCollection) {
    _styleCollection = styleCollection;
    _styleCollection->setParent(this);
    emit styleCollectionChanged();
  }

  QQmlComponent *nodePainter() { return _nodePainter.get(); }
  void setNodePainter(std::unique_ptr<QQmlComponent> nodePainter) {
    _nodePainter = std::move(nodePainter);
    emit nodePainterChanged();
  }

  QQmlComponent *connectionPainter() { return _connectionPainter.get(); }
  void setConnectionPainter(std::unique_ptr<QQmlComponent> connectionPainter) {
    _connectionPainter = std::move(connectionPainter);
    emit nodePainterChanged();
  }

  public:
  signals:
  void styleCollectionChanged();
  void nodePainterChanged();
  void connectionPainterChanged();

  protected:
  AbstractGraphModel *graphModel() { return _graphModel; }

  AbstractGraphModel *_graphModel;
  std::unique_ptr<QQmlComponent> _nodePainter;
  std::unique_ptr<QQmlComponent> _connectionPainter;
  StyleCollection *_styleCollection;
};