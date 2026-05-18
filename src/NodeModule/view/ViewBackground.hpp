#pragma once

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>
#include <qbrush.h>
#include <qcolor.h>

// TODO: convert to a QML Canvas
class ViewBackground : public QQuickPaintedItem {
  Q_OBJECT
  QML_ELEMENT

  public:
  ViewBackground(QQuickItem *parent = nullptr);

  void paint(QPainter *painter) override;
};