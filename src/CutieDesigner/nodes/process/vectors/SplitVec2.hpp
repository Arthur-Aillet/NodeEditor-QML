#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <memory>

class SplitVec2 : public NodeDelegateModel {
  public:
  SplitVec2(QQmlEngine *engine);
  ~SplitVec2() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Split Vec2"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  private:
  double _x = 0.0;
  double _y = 0.0;
  std::shared_ptr<DecimalData> _xData;
  std::shared_ptr<DecimalData> _yData;
};
