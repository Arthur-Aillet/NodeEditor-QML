#pragma once

#include "ATypeCharacterData.hpp"
#include "ATypeCharacterNodeModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qcolor.h>
#include <qcontainerfwd.h>
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
  Q_PROPERTY(float k READ getK WRITE setK NOTIFY kChanged)
  Q_PROPERTY(float scale READ getScale WRITE setScale NOTIFY scaleChanged)
  Q_PROPERTY(
      float smoothFactor READ getSmoothFactor WRITE setSmoothFactor NOTIFY smoothFactorChanged)
  Q_PROPERTY(float fill READ getFill WRITE setFill NOTIFY fillChanged)
  Q_PROPERTY(
      bool substraction READ getSubstraction WRITE setSubstraction NOTIFY substractionChanged)

  UkrugNode(QQmlEngine *engine);
  ~UkrugNode() = default;

  QString caption() const override { return QStringLiteral("Ukrug Character"); }
  bool captionVisible() const override { return true; }
  QString name() const override { return QStringLiteral("Ukrug Character"); }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent *getComponent() override;
  QVariantMap getAdditionalProperties() override;

  QColor getBaseColor() { return _baseColor; }
  float getK() { return _k; }
  float getScale() { return _scale; }
  float getSmoothFactor() { return _smoothFactor; }
  float getFill() { return _fill; }
  bool getSubstraction() { return _substraction; }

  void setBaseColor(QColor baseColor) {
    _baseColor = baseColor;
    emit baseColorChanged();
  }
  void setK(float k) {
    _k = k;
    emit kChanged();
  }
  void setScale(float scale) {
    _scale = scale;
    emit scaleChanged();
  }
  void setSmoothFactor(float smoothFactor) {
    _smoothFactor = smoothFactor;
    emit smoothFactorChanged();
  }
  void setFill(float fill) {
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
  float _k = 0.02;
  float _scale = 0.22;
  float _smoothFactor = 0.01;
  float _fill = 0.018;
  bool _substraction = true;

  std::unique_ptr<QQmlComponent> _component;
  std::shared_ptr<ATypeCharacterData> _modelData;
};
