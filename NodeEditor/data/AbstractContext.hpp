#pragma once

#include "AbstractGraphModel.hpp"
#include "StyleCollection.hpp"

#include <memory>

#include <QQmlComponent>
#include <QQmlEngine>
#include <QtQml>
#include <QtQmlIntegration>

namespace NodeEditor {
class AbstractContext : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Context need to be provided from C++")

  public:
  Q_PROPERTY(NodeEditor::AbstractGraphModel *graphModel READ graphModel CONSTANT)
  Q_PROPERTY(QQmlComponent *nodePainter READ nodePainter NOTIFY nodePainterChanged)
  Q_PROPERTY(
      QQmlComponent *connectionPainter READ connectionPainter NOTIFY connectionPainterChanged)
  Q_PROPERTY(NodeEditor::StyleCollection *styles READ styleCollection NOTIFY styleCollectionChanged)

  AbstractContext(AbstractGraphModel *graphModel, QQmlEngine *engine);

  StyleCollection *styleCollection();
  void setStyleCollection(StyleCollection *styleCollection);

  QQmlComponent *nodePainter();
  void setNodePainter(std::unique_ptr<QQmlComponent> nodePainter);

  QQmlComponent *connectionPainter();
  void setConnectionPainter(std::unique_ptr<QQmlComponent> connectionPainter);

  public:
  signals:
  void styleCollectionChanged();
  void nodePainterChanged();
  void connectionPainterChanged();

  protected:
  AbstractGraphModel *graphModel();

  AbstractGraphModel *_graphModel;
  std::unique_ptr<QQmlComponent> _nodePainter;
  std::unique_ptr<QQmlComponent> _connectionPainter;
  StyleCollection *_styleCollection;
};
}; // namespace NodeEditor
