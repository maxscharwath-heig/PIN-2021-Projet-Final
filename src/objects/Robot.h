#ifndef PIN_2021_SERIE_1_ROBOT_H
#define PIN_2021_SERIE_1_ROBOT_H

#include <FL/Fl_Widget.H>
#include "../core/Canvas.h"
#include "Object.h"

class Robot : public Object {
private:
   const int radius;
   double orientation;
   int leftSpeed, rightSpeed;

public:
   Robot(Position position, int radius, int orientation, int leftSpeed,
         int rightSpeed);

   void draw(WorldWidget* widget) const;

   void update(WorldWidget* widget, double delaTime);
};


#endif //PIN_2021_SERIE_1_ROBOT_H
