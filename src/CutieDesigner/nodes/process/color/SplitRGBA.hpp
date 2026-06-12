#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <memory>
#include <qvariant.h>

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
  std::weak_ptr<NodeData> _inColor;
  QProperty<QVariant> _r = QProperty<QVariant>(double(0.0));
  QProperty<QVariant> _g = QProperty<QVariant>(double(0.0));
  QProperty<QVariant> _b = QProperty<QVariant>(double(0.0));
  QProperty<QVariant> _a = QProperty<QVariant>(double(1.0));
  std::shared_ptr<DecimalData> _rPtr;
  std::shared_ptr<DecimalData> _gPtr;
  std::shared_ptr<DecimalData> _bPtr;
  std::shared_ptr<DecimalData> _aPtr;
};
