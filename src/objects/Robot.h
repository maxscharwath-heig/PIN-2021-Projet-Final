#ifndef PIN_2021_SERIE_1_ROBOT_H
#define PIN_2021_SERIE_1_ROBOT_H

#include <FL/Fl_Widget.H>
#include "../core/Canvas.h"
#include "Object.h"
#include "Particule.h"
#include <queue>

struct RobotData;
struct RobotAction;

class Robot : public Object {
private:
   const int radius;
   double orientation;
   double leftSpeed, rightSpeed;
   std::queue<RobotAction> actions;

   double getAlignementWithParticle(Particule* particule);

   bool canAspirateParticle(Particule* particule);

   bool isInContactWithParticle(Particule* particule);

   void limitWheelConstraint(double& vg, double& vd);

public:
   Robot(Position position, int radius, int orientation, int leftSpeed,
         int rightSpeed);

   void draw(WorldWidget* widget) const override;

   void update(WorldWidget* widget, double delaTime) override;

   RobotData* predict(WorldWidget* widget, double deltaTime);

   static bool collision(RobotData* a, RobotData* b);

   bool collided = false;

   void stop();

   bool addAction(double t, double vg, double vd);

   double goToPosition(int speed, Position destination);

   double goToPositionDuration(double t, Position destination);

   void rotate(double time, double orientation);
};

struct RobotData {
   Robot* robot = nullptr;
   Position position;
   double orientation = 0;
};

struct RobotAction {
   double time = 0;
   double vg = 0;
   double vd = 0;
};

#endif //PIN_2021_SERIE_1_ROBOT_H
