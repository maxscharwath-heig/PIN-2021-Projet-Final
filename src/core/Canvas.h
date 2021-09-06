#ifndef PIN_2021_SERIE_1_CANVAS_H
#define PIN_2021_SERIE_1_CANVAS_H

#include <FL/Fl_Widget.H>
#include <array>
#include "Scale.h"

class Canvas {
public:
   Canvas(int xMin, int yMin, int xMax, int yMax, unsigned tMaxV, int pMin, int pMax,
          unsigned tMaxE, int color = 1);

   void draw(Fl_Widget* parent, Scale* scale) const;

   int x() const noexcept;

   int y() const noexcept;

   int w(double scale) const noexcept;

   int h(double scale) const noexcept;

   unsigned TMAXE() const noexcept;

private:
   int xMin;
   int xMax;
   int yMin;
   int yMax;
   unsigned tMaxV;
   int pMin;
   int pMax;
   int color;
   unsigned tMaxE;

};

#endif //PIN_2021_SERIE_1_CANVAS_H
