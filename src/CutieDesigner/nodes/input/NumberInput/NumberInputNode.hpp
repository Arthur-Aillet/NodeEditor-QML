#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

class NumberInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  NumberInputNode(QQmlEngine *engine);
  ~NumberInputNode() override = default;

  bool captionVisible() const override { return false; }
  QString name() const override { return "Number"; }

  QJsonObject save() const override;
  void load(QJsonObject const &p) override;

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  void embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) override;

  public slots:
  void onTextEdited();

  private:
  double _number = 0.0;
  std::shared_ptr<QQuickItem> _portLabel{nullptr};
  std::shared_ptr<DecimalData> _numberData;
};