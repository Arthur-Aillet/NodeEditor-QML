#pragma once

#include "NodeData.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"
#include <qcolor.h>

class MaskNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *image READ image NOTIFY imageChanged)
  Q_PROPERTY(SurfaceData *mask READ mask NOTIFY maskChanged)
  Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
  Q_PROPERTY(bool inverted READ inverted WRITE setInverted NOTIFY invertedChanged)
  Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
  Q_PROPERTY(float spreadMin READ spreadMin NOTIFY spreadMinChanged)
  Q_PROPERTY(float spreadMax READ spreadMax NOTIFY spreadMaxChanged)
  Q_PROPERTY(float tresholdMin READ tresholdMin NOTIFY tresholdMinChanged)
  Q_PROPERTY(float tresholdMax READ tresholdMax NOTIFY tresholdMaxChanged)

  MaskNode(QQmlEngine *engine);
  ~MaskNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Mask"; }

  QJsonObject save() const override;
  void load(QJsonObject const &p) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "MaskControl");
  }

  QVariantMap componentInitialProperties() override {
    return QVariantMap{{"node", QVariant::fromValue(this)}};
  }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *image() { return _image.lock().get(); }
  SurfaceData *mask() { return _mask.lock().get(); }
  int mode() { return _mode; }

  void setMode(int mode) {
    if (_mode == mode)
      return;
    _mode = mode;
    emit modeChanged();
  }

  int inverted() { return _inverted; }

  void setInverted(int inverted) {
    if (_inverted == inverted)
      return;
    _inverted = inverted;
    emit invertedChanged();
  }

  QColor color() { return _color; }
  float spreadMin() { return _spreadMin; }
  float spreadMax() { return _spreadMax; }
  float tresholdMin() { return _tresholdMin; }
  float tresholdMax() { return _tresholdMax; }

  signals:
  void imageChanged();
  void maskChanged();
  void modeChanged();
  void invertedChanged();
  void colorChanged();
  void spreadMinChanged();
  void spreadMaxChanged();
  void tresholdMinChanged();
  void tresholdMaxChanged();

  private:
  int _mode = 0;
  bool _inverted = false;
  QColor _color = QColor("red");
  float _spreadMin = 0.0;
  float _spreadMax = 1.0;
  float _tresholdMin = 0.0;
  float _tresholdMax = 0.5;
  std::weak_ptr<SurfaceData> _image;
  std::weak_ptr<SurfaceData> _mask;
  std::shared_ptr<SurfaceData> _content;
};
