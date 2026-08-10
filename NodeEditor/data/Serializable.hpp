#pragma once

#include <QtCore/QJsonObject>

class Serializable {
  public:
  virtual ~Serializable() = default;

  virtual QJsonObject save() const { return {}; }

  virtual void load(QJsonObject const & /*p*/) {}
};
