#include "Robot.h"
#include <FL/FL_Draw.H>
#include <cmath>
#include <iostream>
#include "../core/World.h"
#include "../widgets/WorldWidget.h"
#include "../utils/Logger.h"
#include "../utils/Utils.h"

Robot::Robot(Position position, int radius, int orientation, int leftSpeed,
             int rightSpeed) :
      Object(position),
      radius(radius),
      orientation(orientation),
      leftSpeed(leftSpeed),
      rightSpeed(rightSpeed) {
}

void Robot::draw(WorldWidget* widget) const {
   double scale = widget->scale->distance();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();

   fl_color(collided ? FL_GREEN : FL_BLACK);
   //body
   fl_circle(widget->x() + (position.x + offsetX) * scale,
             widget->y() + (position.y + offsetY) * scale,
             radius * scale);
   //head
   double headSize = radius / 4.0;
   double a = to_rad(orientation);
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
         a = to_rad(prediction->orientation),
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
      prediction->orientation += to_deg(w);
   }
   prediction->position.x += dX;
   prediction->position.y += dY;

   return prediction;
}

void Robot::update(WorldWidget* widget, double deltaTime) {

   //goToPosition(100, world->particules.front()->getPosition());

   if (!actions.empty()) {
      auto last = actions.front();
      if (world->simulationTime >= last.time) {
         std::cout << "new move" << std::endl;
         leftSpeed = last.vg;
         rightSpeed = last.vd;
         actions.pop();
      }
   }
   auto prediction = predict(widget, deltaTime);
   position = prediction->position;
   orientation = prediction->orientation;
}

bool Robot::collision(RobotData* a, RobotData* b) {
   double distance = a->position.getDistance(b->position);
   return distance <= a->robot->radius + b->robot->radius;
}

void Robot::stop() {
   leftSpeed = 0;
   rightSpeed = 0;
}

bool Robot::addAction(double t, double vg, double vd) {
   bool useConstraint = false;

   if (!world)return false;

   if (t < 0 || world && t < world->simulationTime) {
      Logger::Log(LoggerType::WARNING, "Cant send a command in the past");
      return false;
   }
   if (!actions.empty() && t <= actions.back().time) {
      Logger::Log(LoggerType::WARNING,
                  "Need to send a command after the last");
      return false;
   }

   if (useConstraint) {
      if (vg <= -world->constraint.vMin || vd <= -world->constraint.vMin) {
         Logger::Log(LoggerType::WARNING, "Error vMin");
         return false;
      }
      if ((vg + vd) / 2 >= world->constraint.vMax) {
         Logger::Log(LoggerType::WARNING, "Error vMax");
         return false;
      }
      if (!actions.empty() &&
          abs(actions.back().time - t) < world->constraint.dtMin) {
         Logger::Log(LoggerType::WARNING,
                     "Need to wait more");
         return false;
      }
      if (!actions.empty() &&
          abs(vg - actions.back().vg) >= world->constraint.dvMax ||
          abs(vd - actions.back().vd) >= world->constraint.dvMax) {
         Logger::Log(LoggerType::WARNING,
                     "Velocity too big");
         return false;
      }
   }

   actions.push({t, vg, vd});
   return true;
}

/**
 * rotate(deg, vitesse){
 *  addAction(0, vitesse, -vitesse);
 *  double t = deg*PI/180.0/vitesse;
 *  addAction(0+t, 0,0);
 * }
 */

bool Robot::canAspirateParticle(Particule* particule) {
   return abs(getAlignementWithParticle(particule)) <= 1.0 &&
          position.getDistance(particule->getPosition()) <=
          radius + particule->getRadius();
}

double Robot::getAlignementWithParticle(Particule* particule) {
   double angle = orientation - particule->getPosition().getAngle(this->position);
   if (angle > 180)
      angle -= 360;
   else if (angle < -180)
      angle += 360;
   return angle;
}

double Robot::goToPosition(int speed, Position destination) {
   double a = to_rad(orientation);
   Position d = {cos(a), sin(a)};
   double distance = position.getDistance(destination);
   double R = (sqrt(d.x * d.x + d.y * d.y) * distance * distance / 2) /
              (-d.y * (destination.x - position.x) +
               d.x * (destination.y - position.y));


   rightSpeed = speed;
   leftSpeed = ((R + radius) / (R - radius) * speed);

   double w = (leftSpeed - rightSpeed) / (2 * radius);
   double t = w == 0 ? distance / leftSpeed : (2.0 * asin((distance / 2.0) / R)) / w;
   double newAngle = to_deg(w * t);
   std::cout << "Robot go to " << destination.x << ":" << destination.y
             << ", arriving in " << t << " s with an angle of " << newAngle << "deg"
             << std::endl;
   return t;
}

void Robot::goToPositionDuration(double time, Position destination) {
   double a = to_rad(orientation);
   Position d = {cos(a), sin(a)};
   double distance = position.getDistance(destination);
   double R = (sqrt(d.x * d.x + d.y * d.y) * distance * distance / 2) /
              (-d.y * (destination.x - position.x) +
               d.x * (destination.y - position.y));


   double w = (2.0 * asin((distance / 2.0) / R)) / time;
   double vt = w * R;
   rightSpeed = (2 * vt) / ((R + radius) / (R - radius) + 1);
   leftSpeed = (2 * vt) - rightSpeed;

   std::cout << "Robot go to " << destination.x << ":" << destination.y
             << ", leftSpeed " << leftSpeed << " rightSpeed " << rightSpeed
             << std::endl;
}

void Robot::rotate(double time, double newOrientation) {
   if (newOrientation == orientation)return;
   double a = to_rad(fmod((newOrientation - orientation) + 180, 360) - 180.0);
   double w = a / time;
   double vg = w * radius;
   std::cout << a << " " << vg << " " << -vg << std::endl;
   addAction(0, vg, -vg);
   addAction(time, 0, 0);
}
