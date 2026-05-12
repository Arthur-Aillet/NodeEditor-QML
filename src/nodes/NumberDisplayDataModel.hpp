#pragma once

#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

#include "DecimalData.hpp"

class QLabel;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NumberDisplayDataModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  NumberDisplayDataModel();

  ~NumberDisplayDataModel() = default;

  public:
  QString caption() const override { return QStringLiteral("Result"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("Result"); }

  public:
  unsigned int nPorts(PortType portType) const override;

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  std::shared_ptr<QQmlComponent> embeddedComponent(QQmlEngine *engine) override {
    if (_component == nullptr) {
      _component = std::make_shared<QQmlComponent>(engine, "CutieDesignerModule", "PortLabel");
    }

    return _component;
  }

  double number() const;

  signals:
  void valueUpdated(double newValue);

  private:
  std::shared_ptr<DecimalData> _numberData;

  std::shared_ptr<QQmlComponent> _component{nullptr};
};
