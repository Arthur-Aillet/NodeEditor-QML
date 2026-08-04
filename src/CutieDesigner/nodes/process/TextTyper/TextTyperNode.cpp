#include "TextTyperNode.hpp"
#include "DecimalData.hpp"
#include "TextData.hpp"
#include "TextTypeEvent.hpp"
#include <qtmetamacros.h>

TextTyperNode::TextTyperNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(std::make_shared<TextData>(_text)), _timer(QTimer()) {};

QJsonObject TextTyperNode::save() const {
  return {{"text", _text}, {"eventList", _eventList.save()["eventList"]}};
}

void TextTyperNode::load(QJsonObject const &json) {
  QJsonValue valueText = json["text"];

  if (!valueText.isUndefined()) {
    _text = valueText.toString();
  }
  _eventList.load(json);
}

QString TextTyperNode::portCaption(PortType portType, PortIndex index) const {
  switch (portType) {
  case NodeEditor::PortType::In:
    if (index == 0)
      return QString("Type delay");
    if (index == 1)
      return QString("Speed scale");
  case NodeEditor::PortType::Out:
    return QString("Text");
  default:
    return QString();
  }
}

unsigned int TextTyperNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

NodeDataType TextTyperNode::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return TextData().type();
  }
}

std::shared_ptr<NodeData> TextTyperNode::outData(PortIndex port) { return _content; };

void TextTyperNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  qDebug() << data.get();
};

QQmlComponent TextTyperNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Process", "TextTyperControl");
}

QVariantMap TextTyperNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
}

TextTyperEventList *TextTyperNode::getModel() { return &_eventList; }

bool TextTyperNode::getPlay() { return _playing; }
void TextTyperNode::setPlay(bool playState) {
  if (playState == _playing)
    return;

  _playing = playState;

  if (playState) {
    // TODO: Prevent Starting with an eventloop instantanous (ex: Only Insert Text with length 0)
    // this cause a crash
    // TODO: Resume the current without starting from scratch the current event
    if (_eventList.events.isEmpty())
      _playing = false;
    else
      processEvent();
  } else {
    _timer.disconnect();
    _timer.stop();
    _currentEventIdx = 0;
  }
  emit playChanged();
}

QString TextTyperNode::getText() { return _text; }
void TextTyperNode::setText(QString newText) {
  _text = newText;
  emit dataUpdated(0);
}

void TextTyperNode::processEvent() {
  _currentEvent = _eventList.events[_currentEventIdx];

  auto visitor = overload{
      [this](const Wait &w) { processWait(); }, [this](const Erase &e) { processErase(); },
      [this](const Replace &r) { processReplace(); }, [this](const Insert &i) { processInsert(); }};
  std::visit(visitor, _eventList.events[_currentEventIdx].value());
}

void TextTyperNode::processNextEvent() {
  _currentEventIdx++;

  if (_currentEventIdx >= _eventList.events.length())
    _currentEventIdx = 0;
  processEvent();
}

void TextTyperNode::processWait() {
  const auto &w = std::get<Wait>(_currentEvent.value());
  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processNextEvent()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(w.delay * 1000);
}

void TextTyperNode::processErase() {
  auto &e = std::get<Erase>(_currentEvent.value());

  if (e.amount == 0 || _text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_text.length(), e.pos);
  _text.removeAt(pos);
  Q_EMIT textChanged();
  Q_EMIT dataUpdated(0);

  e.amount -= 1;

  if (e.amount == 0 || _text.isEmpty())
    return processNextEvent();

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processErase()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(100);
}

void TextTyperNode::processReplace() {
  auto &r = std::get<Replace>(_currentEvent.value());

  if (r.text.isEmpty() || _text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_text.length(), r.pos);
  _text[pos] = r.text[0];
  Q_EMIT textChanged();
  Q_EMIT dataUpdated(0);

  r.text.removeFirst();
  r.pos += 1;

  if (r.text.isEmpty() || _text.isEmpty())
    return processNextEvent();

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processReplace()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(100);
}

void TextTyperNode::processInsert() {
  auto &i = std::get<Insert>(_currentEvent.value());

  if (i.text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_text.length(), i.pos);
  _text.insert(pos, i.text[0]);
  Q_EMIT textChanged();
  Q_EMIT dataUpdated(0);

  i.text.removeFirst();
  i.pos += 1;

  if (i.text.isEmpty())
    return processNextEvent();

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processInsert()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(100);
}