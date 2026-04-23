#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

class NumberData;

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel2 : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel2() {}

  ~ValueNodeModel2() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source2"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource2"); }

  public:
  QJsonObject save() const override {}

  void load(QJsonObject const &p) override {}

  public:
  unsigned int nPorts(PortType portType) const override {}

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {}

  std::shared_ptr<NodeData> outData(PortIndex port) override {}

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override {}

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string) {}

  private:
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel3 : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel3() {}

  ~ValueNodeModel3() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source3"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource3"); }

  public:
  QJsonObject save() const override {}

  void load(QJsonObject const &p) override {}

  public:
  unsigned int nPorts(PortType portType) const override {}

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {}

  std::shared_ptr<NodeData> outData(PortIndex port) override {}

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override {}

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string) {}

  private:
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel4 : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel4() {}

  ~ValueNodeModel4() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source4"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource4"); }

  public:
  QJsonObject save() const override {}

  void load(QJsonObject const &p) override {}

  public:
  unsigned int nPorts(PortType portType) const override {}

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {}

  std::shared_ptr<NodeData> outData(PortIndex port) override {}

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override {}

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string) {}

  private:
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel5 : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel5() {}

  ~ValueNodeModel5() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source5"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource5"); }

  public:
  QJsonObject save() const override {}

  void load(QJsonObject const &p) override {}

  public:
  unsigned int nPorts(PortType portType) const override {}

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {}

  std::shared_ptr<NodeData> outData(PortIndex port) override {}

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override {}

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string) {}

  private:
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel1 : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel1() {}

  ~ValueNodeModel1() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source1"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource1"); }

  public:
  QJsonObject save() const override {}

  void load(QJsonObject const &p) override {}

  public:
  unsigned int nPorts(PortType portType) const override {}

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override {}

  std::shared_ptr<NodeData> outData(PortIndex port) override {}

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QWidget *embeddedWidget() override {}

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited(QString const &string) {}

  private:
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};

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
  std::shared_ptr<NumberData> _number;

  QLineEdit *_lineEdit;
};