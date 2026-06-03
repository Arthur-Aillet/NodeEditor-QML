#pragma once

#include "ATypeCharacterData.hpp"
#include "ATypeCharacterNodeModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qcolor.h>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class UkrugNode : public ATypeCharacterNodeModel {
  Q_OBJECT
  QML_IMPLEMENTS_INTERFACES(ATypeCharacterNodeModel)
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  Q_PROPERTY(QColor baseColor READ getBaseColor WRITE setBaseColor NOTIFY baseColorChanged)
  Q_PROPERTY(double k READ getK WRITE setK NOTIFY kChanged)
  Q_PROPERTY(double scale READ getScale WRITE setScale NOTIFY scaleChanged)
  Q_PROPERTY(
      double smoothFactor READ getSmoothFactor WRITE setSmoothFactor NOTIFY smoothFactorChanged)
  Q_PROPERTY(double fill READ getFill WRITE setFill NOTIFY fillChanged)
  Q_PROPERTY(
      bool substraction READ getSubstraction WRITE setSubstraction NOTIFY substractionChanged)

  UkrugNode(QQmlEngine *engine);
  ~UkrugNode() = default;

  QString caption() const override { return QStringLiteral("Ukrug Character"); }
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Ukrug Character"); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesignerModule", "UkrugControl");
  }

  QVariantMap componentInitialProperties() override {
    QVariantMap map;
    map["node"] = QVariant::fromValue(this);
    return map;
  }

  QQmlComponent *getComponent() override;
  QVariantMap getAdditionalProperties() override;

  QColor getBaseColor() { return _baseColor; }
  double getK() { return _k; }
  double getScale() { return _scale; }
  double getSmoothFactor() { return _smoothFactor; }
  double getFill() { return _fill; }
  bool getSubstraction() { return _substraction; }

  void setBaseColor(QColor baseColor) {
    _baseColor = baseColor;
    emit baseColorChanged();
  }
  void setK(double k) {
    _k = k;
    emit kChanged();
  }
  void setScale(double scale) {
    _scale = scale;
    emit scaleChanged();
  }
  void setSmoothFactor(double smoothFactor) {
    _smoothFactor = smoothFactor;
    emit smoothFactorChanged();
  }
  void setFill(double fill) {
    _fill = fill;
    emit fillChanged();
  }
  void setSubstraction(bool substraction) {
    _substraction = substraction;
    emit substractionChanged();
  }

  signals:
  void baseColorChanged();
  void kChanged();
  void scaleChanged();
  void smoothFactorChanged();
  void fillChanged();
  void substractionChanged();

  private:
  QColor _baseColor = "white";
  double _k = 0.02;
  double _scale = 0.22;
  double _smoothFactor = 0.01;
  double _fill = 0.018;
  bool _substraction = true;

  std::unique_ptr<QQmlComponent> _component;
  std::shared_ptr<ATypeCharacterData> _modelData;
};
