#include "Robot.h"
#include <FL/FL_Draw.H>
#include <cmath>
#include <iostream>
#include "../core/World.h"
#include "../widgets/WorldWidget.h"

Robot::Robot(Position position, int radius, int orientation, int leftSpeed,
             int rightSpeed) :
      Object(position),
      radius(radius),
      orientation(orientation),
      leftSpeed(leftSpeed),
      rightSpeed(rightSpeed) {}

void Robot::draw(WorldWidget* widget) const {
   double ratio = widget->scale->getRatio();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();

   fl_color(FL_BLACK);
   //body
   fl_circle(widget->x() + (position.x + offsetX) * ratio,
             widget->y() + (position.y + offsetY) * ratio,
             radius * ratio);
   //head
   int headSize = radius / 4;
   double a = orientation * M_PI / 180.0;
   double x1 = position.x + ((radius - headSize * 2) * cos(a));
   double y1 = position.y + ((radius - headSize * 2) * sin(a));

   fl_circle(widget->x() + (x1 + offsetX) * ratio,
             widget->y() + (y1 + offsetY) * ratio,
             headSize * ratio);
}

void Robot::update(WorldWidget* widget, double deltaTime) {
   double
         t = deltaTime,
         dX = 0,
         dY = 0,
         a = orientation * M_PI / 180.0,
         vg = leftSpeed,
         vd = rightSpeed;

   /** THE FORMULA ON DOC IS COMPLETELY FU**ED UP... SORRY NOT SORRY
    * pMax and pMin are the same value so it just force speed to be these value so
    * the robot go always in linear movement...
   {//bounding wheel speed
      double
            maxS = double(widget->canvas->pMax) / widget->canvas->tMaxV,
            minS = double(widget->canvas->pMin) / widget->canvas->tMaxV;
      vg = fmax(minS, fmin(abs(vg), maxS)) * ((vg < 0) ? -1.0 : 1.0);
      vd = fmax(minS, fmin(abs(vd), maxS)) * ((vd < 0) ? -1.0 : 1.0);
   }
    **/

   if (vg == vd) {//linear
      dX = vg * cos(a) * t;
      dY = vg * sin(a) * t;
   } else if (vg == -vd) {//rotate
      orientation += vg * t;
   } else {//circle
      double R = radius * (vg + vd) / (vg - vd);
      double vT = (vg + vd) / 2.0;
      double w = vT / R * t;
      dX = R * (cos(a) * sin(w) - sin(a) * (1 - cos(w)));
      dY = R * (sin(a) * sin(w) + cos(a) * (1 - cos(w)));
      orientation += w * 180.0 / M_PI;
   }
   position.x += dX;
   position.y += dY;
}
