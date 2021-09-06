#ifndef PIN_2021_SERIE_1_ROBOT_H
#define PIN_2021_SERIE_1_ROBOT_H

#include <FL/Fl_Widget.H>
#include "../core/Canvas.h"
#include "Object.h"
#include "Particule.h"
#include <queue>

struct RobotData;
struct RobotAction;
enum class RobotEventState { // priority LOW -> HIGH
   UNKNOWN = 0,
   PARTICULE_CONTACT,
   NO_PARTICULE,
   COLLISION_WARNING,
};

struct RobotEvent {
   RobotEventState event = RobotEventState::UNKNOWN;
   void* data = nullptr;
};

class Robot : public Object {
private:
   const int radius;
   double leftSpeed, rightSpeed;
   std::queue<RobotAction> actions;
   RobotEvent event;

   bool canAspirateParticle(Particule* particule) const;

   bool isInContactWithParticle(Particule* particule) const;

   void limitWheelConstraint(double& vg, double& vd);

   void setEvent(RobotEventState newEvent, void* data = nullptr);

public:
   Robot(Position position, int radius, int orientation, int leftSpeed,
         int rightSpeed);

   double getAlignementWithParticle(Particule* particule) const;

   void draw(WorldWidget* widget) const override;

   void update(WorldWidget* widget, double delaTime) override;

   RobotData* predict(WorldWidget* widget, double deltaTime);

   static bool collision(RobotData* a, RobotData* b);

   bool isEmergencyStopped = false;

   void stop() noexcept;

   bool addAction(double t, double vg, double vd);

   std::tuple<double,double,double> goToPosition(int speed, Position destination);

   std::tuple<double,double,double> goToPositionDuration(double t, Position destination);

   void rotate(double time, double orientation);

   void emergencyStop();

   int getRadius() const noexcept override;

   double getLSpeed() const;

   double getRSpeed() const;

   RobotEvent getEvent() const noexcept;

   void resetEvent();

   void aspirate(Particule* particule);

   void setTarget(Particule* target);

   Particule* getTarget() const noexcept;

   bool hasTarget() const noexcept;

   friend bool operator<(const Robot& a, const Robot& b);

   std::string infos() const;

   void speedLog() const;

   void clearAction();

   double getOrientation() const noexcept;

   Particule* target;
   double orientation;

   bool isRotating() const;

   void rotate(double speed);
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
