#pragma once

#include "NodeDelegateModel.hpp"
#include "nodes/DecimalData.hpp"

#include <QtCore/QObject>

class NumberData;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel();

  ~ValueNodeModel() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource"); }

  public:
  QJsonObject save() const override;

  void load(QJsonObject const &p) override;

  public:
  unsigned int nPorts(PortType portType) const override;

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override;

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string);

  private:
  std::shared_ptr<DecimalData> _number;

  QLineEdit *_lineEdit;
};