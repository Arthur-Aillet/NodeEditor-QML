#pragma once

#include "AbstractGraphModel.hpp"

#include <memory>

#include <QQmlComponent>
#include <QQmlEngine>
#include <QtQml>
#include <QtQmlIntegration>

class NodeEditorContext : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provied from C++")

  public:
  Q_PROPERTY(AbstractGraphModel *graphModel READ graphModel CONSTANT)
  Q_PROPERTY(QQmlComponent *nodePainter READ nodePainter CONSTANT)
  Q_PROPERTY(QQmlComponent *connectionPainter READ connectionPainter CONSTANT)
  // Q_PROPERTY(StyleCollection styleCollection MEMBER _styleCollection CONSTANT)

  NodeEditorContext(AbstractGraphModel *graphModel, QQmlEngine *engine)
      : _graphModel(graphModel), QObject(engine) {
    _nodePainter = std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultNodePainter");
    _connectionPainter =
        std::make_unique<QQmlComponent>(engine, "NodeEditor", "DefaultConnectionPainter");
    // _styleCollection = std::make_unique<StyleCollection>();
  };

  private:
  AbstractGraphModel *graphModel() { return _graphModel; }
  QQmlComponent *nodePainter() { return _nodePainter.get(); }
  QQmlComponent *connectionPainter() { return _connectionPainter.get(); }

  AbstractGraphModel *_graphModel;
  std::unique_ptr<QQmlComponent> _nodePainter;
  std::unique_ptr<QQmlComponent> _connectionPainter;
  // std::unique_ptr<StyleCollection> _styleCollection;
};