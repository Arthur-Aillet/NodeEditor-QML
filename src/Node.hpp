#include <QDebug>
#include <qobject.h>
#include <type_traits>

template <typename T>
class Node {
  public:
  static_assert(std::is_base_of<QObject, T>{}, "Node content must be a QObject");

  std::unordered_multimap<std::string, int> driver_mapping;

  Node() {
    auto metaObject = T::staticMetaObject;

    for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); ++i) {
      driver_mapping.insert(std::pair("intensity", i));
    }
  }
};