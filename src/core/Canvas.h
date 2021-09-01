#ifndef PIN_2021_SERIE_1_CANVAS_H
#define PIN_2021_SERIE_1_CANVAS_H

#include <FL/Fl_Widget.H>
#include <array>
#include "Scale.h"

class Canvas {
public:
   Canvas(int xMin, int yMin, int xMax, int yMax, unsigned tMaxV, int pMin, int pMax,
          unsigned tMaxE, int color = 1);

   void draw(Fl_Widget* parent, Canvas* canvas, Scale* scale) const;

   int x() const;

   int w() const;

   int y() const;

   int h() const;

   unsigned TMAXE() const;

   unsigned tMaxV;
   int pMin;
   int pMax;
   unsigned tMaxE;

private:
   int xMin;
   int xMax;
   int yMin;
   int yMax;
   int color;

};


#endif //PIN_2021_SERIE_1_CANVAS_H
