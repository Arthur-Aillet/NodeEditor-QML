#pragma once

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QtGui/QColor>
#include <qbrush.h>

// TODO: convert to a QML Canvas
class ViewBackground : public QQuickPaintedItem {
  Q_OBJECT
  QML_ELEMENT

  public:
  ViewBackground(QQuickItem *parent = nullptr);

  void paint(QPainter *painter) override;
};