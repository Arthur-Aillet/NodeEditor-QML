#pragma once

#include "AbstractGraphModel.hpp"
#include "StyleCollection.hpp"

#include <memory>

#include <QQmlComponent>
#include <QQmlEngine>
#include <QtQml>
#include <QtQmlIntegration>

class AbstractContext : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(AbstractGraphModel *graphModel READ graphModel CONSTANT)
  Q_PROPERTY(QQmlComponent *nodePainter READ nodePainter CONSTANT)
  Q_PROPERTY(QQmlComponent *connectionPainter READ connectionPainter CONSTANT)
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

  public:
  signals:
  void styleCollectionChanged();

  protected:
  AbstractGraphModel *graphModel() { return _graphModel; }
  QQmlComponent *nodePainter() { return _nodePainter.get(); }
  QQmlComponent *connectionPainter() { return _connectionPainter.get(); }

  AbstractGraphModel *_graphModel;
  std::unique_ptr<QQmlComponent> _nodePainter;
  std::unique_ptr<QQmlComponent> _connectionPainter;
  StyleCollection *_styleCollection;
};