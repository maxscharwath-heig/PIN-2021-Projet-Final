#include <FL/fl_draw.H>
#include "Canvas.h"

Canvas::Canvas(int xMin, int yMin, int xMax, int yMax, unsigned tMaxV, int pMin,
               int pMax, unsigned tMaxE, int color) :
      xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), tMaxV(tMaxV), pMin(pMin),
      pMax(pMax), color(color), tMaxE(tMaxE) {}

void Canvas::draw(Fl_Widget* parent, Scale* scale) const {
   fl_color(color);
   fl_rectf(parent->x(), parent->y(), w(scale->distance()), h(scale->distance()));
}

int Canvas::x() const noexcept {
   return -xMin;
}

int Canvas::y() const noexcept {
   return -yMin;
}

int Canvas::w(double scale) const noexcept {
   return (int) ((xMax - xMin) * scale);
}

int Canvas::h(double scale) const noexcept {
   return (int) ((yMax - yMin) * scale);
}

unsigned Canvas::TMAXE() const noexcept {
   return tMaxE;
}


