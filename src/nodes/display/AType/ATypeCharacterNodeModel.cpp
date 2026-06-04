#include "ATypeCharacterNodeModel.hpp"
#include <qdebug.h>
#include <qlogging.h>
#include <qtimer.h>

void ATypeCharacterNodeModel::createCharacterObject(QString initialChar, int index) {
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
  newPtr->setParentItem(_container);
  auto children = _container->childItems();
  if (index < children.size() - 1) {
    newPtr->stackBefore(children.at(index));
  }
  QJSEngine::setObjectOwnership(newPtr.get(), QJSEngine::CppOwnership);
  _characters.push_back(newPtr);
}

void ATypeCharacterNodeModel::destroyItem(int index) {
  auto children = _container->childItems();

  children.at(index)->setProperty("goingToGetDestroyed", true);

  // Weak Pointer is a bit too overkill, but i keep it to prevent eventual crashes down the road
  QWeakPointer<QQuickItem> ptr;

  for (int i = 0; i != _characters.size(); i++) {
    if (_characters[i].get() == children.at(index)) {
      ptr = _characters[i].toWeakRef();
      break;
    }
  }

  auto animationDuration = std::max(_animationOpacitySpeed, _animationWidthSpeed);

  QTimer::singleShot(animationDuration, this, [this, ptr, index]() {
    auto children = _container->childItems();

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

void ATypeCharacterNodeModel::setChar(int index, QString character) {
  auto children = _container->childItems();

  children[index]->setProperty("char", character);
}

QString ATypeCharacterNodeModel::getString() {
  QString result = "";
  auto children = _container->childItems();

  for (auto charac : children) {
    if (charac->property("goingToGetDestroyed") == false)
      result.append(charac->property("char").toString());
  }
  return result;
}