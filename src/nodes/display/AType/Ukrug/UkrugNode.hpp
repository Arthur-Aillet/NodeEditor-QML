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

class UkrugPointsList;

class UkrugNode : public ATypeCharacterNodeModel {
  Q_OBJECT
  QML_IMPLEMENTS_INTERFACES(ATypeCharacterNodeModel)
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  Q_PROPERTY(double k MEMBER _k NOTIFY kChanged)
  Q_PROPERTY(double circleScale MEMBER _circleScale NOTIFY circleScaleChanged)
  Q_PROPERTY(double pointsScale MEMBER _pointsScale NOTIFY pointsScaleChanged)
  Q_PROPERTY(double pointsDistance MEMBER _pointsDistance NOTIFY pointsDistanceChanged)
  Q_PROPERTY(double smoothFactor MEMBER _smoothFactor NOTIFY smoothFactorChanged)
  Q_PROPERTY(double fill MEMBER _fill NOTIFY fillChanged)
  Q_PROPERTY(bool substraction MEMBER _substraction NOTIFY substractionChanged)

  Q_PROPERTY(double boxLimitX MEMBER _boxLimitX NOTIFY boxLimitXChanged)
  Q_PROPERTY(double boxLimitY MEMBER _boxLimitY NOTIFY boxLimitYChanged)
  Q_PROPERTY(double boxRadius MEMBER _boxRadius NOTIFY boxRadiusChanged)

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

  signals:
  void kChanged();
  void circleScaleChanged();
  void pointsDistanceChanged();
  void pointsScaleChanged();
  void smoothFactorChanged();
  void fillChanged();
  void substractionChanged();
  void boxLimitXChanged();
  void boxLimitYChanged();
  void boxRadiusChanged();

  friend UkrugPointsList;

  private:
  double _k = 0.02;
  double _circleScale = 1.0;
  double _pointsScale = 0.22;
  double _smoothFactor = 0.01;
  double _fill = 0.018;
  bool _substraction = true;
  double _pointsDistance = 0.45;
  double _boxLimitX = 1;
  double _boxLimitY = 1;
  double _boxRadius = 0.1;

  std::unique_ptr<QQmlComponent> _component;
  std::shared_ptr<ATypeCharacterData> _modelData;
};
