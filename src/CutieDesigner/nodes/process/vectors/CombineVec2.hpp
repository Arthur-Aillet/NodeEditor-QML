#pragma once

#include "NodeDelegateModel.hpp"
#include "Vec2Data.hpp"

#include <memory>

class CombineVec2 : public NodeDelegateModel {
  public:
  CombineVec2(QQmlEngine *engine);
  ~CombineVec2() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Combine Vec2"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  private:
  QVector2D _vec = QVector2D(0.0, 0.0);
  std::shared_ptr<Vec2Data> _vecData;
};
