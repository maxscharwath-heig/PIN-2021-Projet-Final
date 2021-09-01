//
// Created by Maxime on 31.08.2021.
//

#ifndef PIN_2021_SERIE_1_FL_CENTER_SCROLL_H
#define PIN_2021_SERIE_1_FL_CENTER_SCROLL_H

#include <iostream>
#include <FL/Fl_Scroll.H>

class Fl_Center_Scroll : public Fl_Scroll {
private:
   int prevW = 0;
   int prevH = 0;

   int centerX(Fl_Widget* wid) {
      return x() + (w() - wid->w()) / 2;
   }

   int centerY(Fl_Widget* wid) {
      return y() + (h() - wid->h()) / 2;
   }

public:
   Fl_Center_Scroll(int X, int Y, int W, int H, const char* L = nullptr) : Fl_Scroll(
         X, Y, W, H, L
   ) {
   }

   void draw() {
      if (children() > 0) {
         Fl_Widget* cw = child(0);
         if (prevH != cw->h() || prevW != cw->w()) {
            cw->position(centerX(cw), centerY(cw));
            prevH = cw->h();
            prevW = cw->w();
         }
      }
      Fl_Scroll::draw();
   }

   void resize(int X, int Y, int W, int H) override {
      Fl_Scroll::resize(X, Y, W, H);
      if (children() > 0) {
         Fl_Widget* cw = child(0);
         cw->position((w() - cw->w()) > 0 ? centerX(cw) : cw->x(),
                      (h() - cw->h()) > 0 ? centerY(cw) : cw->y());
      }
   }

   void end() {
      Fl_Group::end();
      resize(x(), y(), w(), h());
   }
};


#endif //PIN_2021_SERIE_1_FL_CENTER_SCROLL_H
