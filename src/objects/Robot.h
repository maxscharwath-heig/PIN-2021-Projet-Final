#ifndef PIN_2021_SERIE_1_ROBOT_H
#define PIN_2021_SERIE_1_ROBOT_H

#include <FL/Fl_Widget.H>
#include "../core/Canvas.h"
#include "Object.h"

struct RobotData;

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

   RobotData* predict(WorldWidget* widget, double deltaTime);

   static bool collision(RobotData* a, RobotData* b);

   bool collided = false;

   void stop();
};

struct RobotData {
   Robot* robot = nullptr;
   Position position;
   double orientation = 0;
};

#endif //PIN_2021_SERIE_1_ROBOT_H