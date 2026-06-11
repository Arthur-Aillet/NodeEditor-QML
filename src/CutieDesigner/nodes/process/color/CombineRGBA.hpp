#pragma once

#include "ColorData.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"
#include <memory>
#include <qcolor.h>

class CombineRGBA : public NodeDelegateModel {
  public:
  CombineRGBA(QQmlEngine *engine);
  ~CombineRGBA() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Combine RGBA"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  private:
  std::weak_ptr<DecimalData> _r;
  std::weak_ptr<DecimalData> _g;
  std::weak_ptr<DecimalData> _b;
  std::weak_ptr<DecimalData> _a;
  QProperty<QColor> _color;
  std::shared_ptr<ColorData> _outColorPtr;
};
