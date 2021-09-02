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
   double scale = widget->scale->distance();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();

   fl_color(collided ? FL_GREEN : FL_BLACK);
   //body
   fl_circle(widget->x() + (position.x + offsetX) * scale,
             widget->y() + (position.y + offsetY) * scale,
             radius  * scale);
   //head
   int headSize = radius / 4;
   double a = orientation * M_PI / 180.0;
   double x1 = position.x + ((radius - headSize * 2) * cos(a));
   double y1 = position.y + ((radius - headSize * 2) * sin(a));

   fl_circle(widget->x() + (x1 + offsetX) * scale,
             widget->y() + (y1 + offsetY) * scale,
             headSize * scale);
}

RobotData* Robot::predict(WorldWidget* widget, double deltaTime) {
   auto* prediction = new RobotData{
         .robot =  this,
         .position = {
               position.x,
               position.y,
         },
         .orientation = orientation
   };
   double
         t = deltaTime,
         dX = 0,
         dY = 0,
         a = prediction->orientation * M_PI / 180.0,
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
   } else {//circle
      double R = radius * (vg + vd) / (vg - vd);
      double w = (vg - vd) / (radius * 2.0) * t;
      dX = R * (cos(a) * sin(w) - sin(a) * (1 - cos(w)));
      dY = R * (sin(a) * sin(w) + cos(a) * (1 - cos(w)));
      prediction->orientation += w * 180.0 / M_PI;
   }
   prediction->position.x += dX;
   prediction->position.y += dY;
   return prediction;
}

void Robot::update(WorldWidget* widget, double deltaTime) {
   auto prediction = predict(widget, deltaTime);
   position = prediction->position;
   orientation = prediction->orientation;
}

bool Robot::collision(RobotData* a, RobotData* b) {
   //return if circle a and circle b are colliding
   double dx = a->position.x - b->position.x;
   double dy = a->position.y - b->position.y;
   double distance = sqrt(dx * dx + dy * dy);
   return distance <= a->robot->radius + b->robot->radius;
}

void Robot::stop() {
   leftSpeed = 0;
   rightSpeed = 0;
}
