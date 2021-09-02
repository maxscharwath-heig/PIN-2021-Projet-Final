#include <FL/fl_draw.H>
#include "Canvas.h"

void Canvas::draw(Fl_Widget* parent, Canvas* canvas, Scale* scale) const {
   fl_color(color);
   fl_rectf(parent->x(), parent->y(), w() * scale->distance(), h() * scale->distance());
}

Canvas::Canvas(int xMin, int yMin, int xMax, int yMax, unsigned tMaxV, int pMin,
               int pMax, unsigned tMaxE, int color) :
      xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), tMaxV(tMaxV), pMin(pMin),
      pMax(pMax), tMaxE(tMaxE), color(color) {}

int Canvas::x() const {
   return xMin;
}

int Canvas::y() const {
   return yMin;
}

int Canvas::w() const {
   return xMax - xMin;
}

int Canvas::h() const {
   return yMax - yMin;
}

unsigned Canvas::TMAXE() const {
   return tMaxE;
}


