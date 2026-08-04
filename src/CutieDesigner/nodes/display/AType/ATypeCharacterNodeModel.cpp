#include "ATypeCharacterNodeModel.hpp"

#include <QColor>

QJsonObject ATypeCharacterNodeModel::save() const {
  QJsonObject json;

  json["baseColor"] = QJsonValue::fromVariant(_baseColor);
  json["fontSize"] = _fontSize;
  json["animationOpacitySpeed"] = _animationOpacitySpeed;
  json["animationWidthSpeed"] = _animationWidthSpeed;
  json["animationTransformSpeed"] = _animationTransformSpeed;
  return json;
}
void ATypeCharacterNodeModel::load(QJsonObject const &json) {
  if (!json["baseColor"].isUndefined())
    _baseColor = json["baseColor"].toVariant().value<QColor>();
  if (!json["fontSize"].isUndefined())
    _fontSize = json["fontSize"].toDouble();
  if (!json["animationOpacitySpeed"].isUndefined())
    _animationOpacitySpeed = json["animationOpacitySpeed"].toDouble();
  if (!json["animationWidthSpeed"].isUndefined())
    _animationWidthSpeed = json["animationWidthSpeed"].toDouble();
  if (!json["animationTransformSpeed"].isUndefined())
    _animationTransformSpeed = json["animationTransformSpeed"].toDouble();
}

void ATypeCharacterNodeModel::createCharacterObject(QQuickItem *instance, QString initialChar,
                                                    int index) {
  auto component = getComponent();

  if (component->isNull()) {
    qWarning() << "AType letter componenent is null";
    return;
  }
  if (component->isError()) {
    qCritical() << "AType letter componenent:" << component->errorString();
    return;
  }
  if (!component->isReady()) {
    qCritical() << "AType letter componenent: Not Ready to be created";
    return;
  }
  auto props = getAdditionalProperties();
  props["char"] = initialChar;

  auto newPtr = QSharedPointer<QQuickItem>(
      qobject_cast<QQuickItem *>(component->createWithInitialProperties(props)));
  newPtr->setParentItem(instance);
  auto children = instance->childItems();
  if (index < children.size() - 1) {
    newPtr->stackBefore(children.at(index));
  }
  QJSEngine::setObjectOwnership(newPtr.get(), QJSEngine::CppOwnership);
  _characters.push_back(newPtr);
}

void ATypeCharacterNodeModel::destroyItem(QQuickItem *instance, int index) {
  auto children = instance->childItems();

  QQuickItem *realChild;
  int realIndex = 0;

  for (int i = 0; i != children.length(); i++) {
    QQuickItem *currentChild = children.at(i);

    if (!currentChild->property("goingToGetDestroyed").toBool()) {
      if (realIndex == index) {
        realChild = currentChild;
        break;
      }
      realIndex += 1;
    }
  }

  realChild->setProperty("goingToGetDestroyed", true);

  // Weak Pointer is a bit too overkill, but i keep it to prevent eventual crashes down the road
  QWeakPointer<QQuickItem> ptr;

  for (int i = 0; i != _characters.size(); i++) {
    if (_characters[i].get() == realChild) {
      ptr = _characters[i].toWeakRef();
      break;
    }
  }

  auto animationDuration = std::max(_animationOpacitySpeed, _animationWidthSpeed);

  QTimer::singleShot(animationDuration, this, [this, ptr, index, instance]() {
    auto children = instance->childItems();

    if (ptr.isNull())
      return;
    for (int i = 0; i != _characters.size(); i++) {
      if (_characters[i].toWeakRef() == ptr) {
        _characters.erase(_characters.begin() + i);
        return;
      }
    }
  });
}

void ATypeCharacterNodeModel::setChar(QQuickItem *instance, int index, QString character) {
  auto children = instance->childItems();

  children[index]->setProperty("char", character);
}

QString ATypeCharacterNodeModel::getString(QQuickItem *instance) {
  QString result = "";
  auto children = instance->childItems();

  for (auto charac : children) {
    if (charac->property("goingToGetDestroyed") == false)
      result.append(charac->property("char").toString());
  }
  return result;
}