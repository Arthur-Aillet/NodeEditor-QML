#pragma once

#include <qabstractitemmodel.h>
#include <qbrush.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class GradientInputList : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  public:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int portsCount() const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Q_INVOKABLE void pushColor();
  Q_INVOKABLE void popColor();
  Q_INVOKABLE void editColor(QColor color, int index);
  Q_INVOKABLE void editPos(double pos, int index);

  enum class GradientInputRole { Pos = Qt::UserRole + 1, Color };

  QGradient gradient;

  signals:
  void gradientChanged();

  protected:
  QGradientStops _gradientStops = QGradientStops();
  QHash<int, QByteArray> roleNames() const override {
    return {{static_cast<int>(GradientInputRole::Pos), "pos"},
            {static_cast<int>(GradientInputRole::Color), "color"}};
  }
};
