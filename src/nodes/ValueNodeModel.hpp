#pragma once

#include "nodes/DecimalData.hpp"
#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

class NumberData;

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;

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
  unsigned int nPorts(QtNodes::PortType portType) const override;

  NodeDataType dataType(QtNodes::PortType portType, PortIndex portIndex) const override;

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