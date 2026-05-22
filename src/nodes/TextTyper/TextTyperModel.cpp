#include "TextTyperModel.hpp"
#include "DecimalData.hpp"
#include "TextData.hpp"

QString TextTyperModel::portCaption(PortType portType, PortIndex index) const {
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

unsigned int TextTyperModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &TextTyperModel::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return TextData().type();
  }
}

std::shared_ptr<NodeData> TextTyperModel::outData(PortIndex port) { return _content; };

void TextTyperModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  qDebug() << data.get();
};

QQmlComponent TextTyperModel::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesignerModule", "TextTyper");
}

QVariantMap TextTyperModel::componentInitialProperties() {
  QVariantMap map;
  map["textTyper"] = QVariant::fromValue(this);
  return map;
}

void TextTyperModel::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _textTyperQml = loaded;
}

TextTyperEventList *TextTyperModel::getModel() { return &_eventList; }

bool TextTyperModel::getPlay() { return _playing; }
void TextTyperModel::setPlay(bool playState) {
  _playing = playState;

  if (playState) {
    if (_eventList.events.isEmpty())
      _playing = false;
    else
      processEvent();
  } else {
    _timer.disconnect();
    _timer.stop();
  }
  Q_EMIT playChanged();
}

QString TextTyperModel::getText() { return _content->text; }
void TextTyperModel::setText(QString newText) {
  _content->text = newText;
  Q_EMIT dataUpdated(0);
}

void TextTyperModel::processEvent() {
  _currentEvent = _eventList.events[_currentEventIdx];

  auto visitor = overload{
      [this](const Wait &w) { processWait(); }, [this](const Erase &e) { processErase(); },
      [this](const Replace &r) { processReplace(); }, [this](const Insert &i) { processInsert(); }};
  std::visit(visitor, _eventList.events[_currentEventIdx]);
}

void TextTyperModel::processNextEvent() {
  _currentEventIdx++;

  if (_currentEventIdx >= _eventList.events.length())
    _currentEventIdx = 0;
  processEvent();
}

void TextTyperModel::processWait() {
  const auto &w = std::get<Wait>(_currentEvent);
  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processNextEvent()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(w.delay * 1000);
}

void TextTyperModel::processErase() {
  auto &e = std::get<Erase>(_currentEvent);

  if (e.amount == 0 || _content->text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_content->text.length(), e.pos);
  _content->text.removeAt(pos);
  Q_EMIT textChanged();
  Q_EMIT dataUpdated(0);

  e.amount -= 1;

  if (e.amount == 0 || _content->text.isEmpty())
    return processNextEvent();

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processErase()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(100);
}

void TextTyperModel::processReplace() {
  auto &r = std::get<Replace>(_currentEvent);

  if (r.text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_content->text.length(), r.pos);
  _content->text[pos] = r.text[0];
  Q_EMIT textChanged();
  Q_EMIT dataUpdated(0);

  r.text.removeFirst();
  r.pos += 1;

  if (r.text.isEmpty())
    return processNextEvent();

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(processReplace()),
                 Qt::ConnectionType::SingleShotConnection);
  _timer.start(100);
}

void TextTyperModel::processInsert() {
  auto &i = std::get<Insert>(_currentEvent);

  if (i.text.isEmpty())
    return processNextEvent();

  uint pos = std::min((uint)_content->text.length(), i.pos);
  _content->text.insert(pos, i.text[0]);
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