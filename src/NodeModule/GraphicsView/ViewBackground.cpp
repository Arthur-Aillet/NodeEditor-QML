#include <QtNodes/StyleCollection>

#include "ViewBackground.hpp"

using QtNodes::StyleCollection;

ViewBackground::ViewBackground(QQuickItem *parent) : QQuickPaintedItem(parent) {
  auto const &flowViewStyle = StyleCollection::flowViewStyle();

  setAntialiasing(false);
  setSmooth(false);
  this->setFillColor(flowViewStyle.BackgroundColor);
}

void ViewBackground::paint(QPainter *painter) {
  auto drawGrid = [&](double gridStep) {
    QRectF windowRect = clipRect();
    QPointF tl = windowRect.topLeft();
    QPointF br = windowRect.bottomRight();

    double left = std::floor(tl.x() / gridStep - 0.5);
    double right = std::floor(br.x() / gridStep + 1.0);
    double bottom = std::floor(tl.y() / gridStep - 0.5);
    double top = std::floor(br.y() / gridStep + 1.0);

    // vertical lines
    for (int xi = int(left); xi <= int(right); ++xi) {
      QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

      painter->drawLine(line);
    }

    // horizontal lines
    for (int yi = int(bottom); yi <= int(top); ++yi) {
      QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
      painter->drawLine(line);
    }
  };

  auto const &flowViewStyle = StyleCollection::flowViewStyle();

  QPen pfine(flowViewStyle.FineGridColor, 1.0);

  painter->setPen(pfine);
  drawGrid(15);

  QPen p(flowViewStyle.CoarseGridColor, 1.0);

  painter->setPen(p);
  drawGrid(150);
};