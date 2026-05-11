#include "StyleCollection.hpp"
#include <QPainter>

#include "ViewBackground.hpp"

ViewBackground::ViewBackground(QQuickItem *parent) : QQuickPaintedItem(parent) {
  auto const &style = StyleCollection::getGraphicsViewStyle();

  this->setFillColor(style.BackgroundColor);
}

void ViewBackground::paint(QPainter *painter) {
  painter->setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, this->smooth());
  painter->setRenderHint(QPainter::RenderHint::Antialiasing, this->antialiasing());

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

  auto const &style = StyleCollection::getGraphicsViewStyle();

  QPen pfine(style.FineGridColor, 1.0);

  painter->setPen(pfine);
  drawGrid(15);

  QPen p(style.CoarseGridColor, 1.0);

  painter->setPen(p);
  drawGrid(150);
};