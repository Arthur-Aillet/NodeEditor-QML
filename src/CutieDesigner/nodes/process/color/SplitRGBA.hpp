#pragma once

#include "ColorData.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <memory>

class SplitRGBA : public NodeDelegateModel {
  public:
  SplitRGBA(QQmlEngine *engine);
  ~SplitRGBA() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Split RGBA"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  private:
  std::weak_ptr<ColorData> _inColor;
  std::shared_ptr<DecimalData> _r;
  std::shared_ptr<DecimalData> _g;
  std::shared_ptr<DecimalData> _b;
  std::shared_ptr<DecimalData> _a;
};
