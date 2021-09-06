#include "Robot.h"
#include <FL/FL_Draw.H>
#include <cmath>
#include <iostream>
#include <sstream>
#include <FL/fl_ask.H>
#include <tuple>
#include "../core/World.h"
#include "../widgets/WorldWidget.h"
#include "../utils/Logger.h"

Robot::Robot(Position position, int radius, int orientation, int leftSpeed,
             int rightSpeed) :
      Object(position),
      radius(radius),
      orientation(orientation),
      leftSpeed(leftSpeed),
      rightSpeed(rightSpeed),
      target(nullptr) {}

void Robot::draw(WorldWidget* widget) const {
   double scale = widget->scale->distance();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();

   fl_color(isEmergencyStopped ? FL_RED : FL_BLACK);
   //body
   fl_circle(widget->x() + (offsetX - position.x) * scale,
             widget->y() + (offsetY - position.y) * scale,
             radius * scale);
   //head
   double headSize = radius / 4.0;
   double a = to_rad(orientation);
   double x1 = position.x + ((radius - headSize * 2) * cos(a));
   double y1 = position.y + ((radius - headSize * 2) * sin(a));

   fl_circle(widget->x() + (offsetX - x1) * scale,
             widget->y() + (offsetY - y1) * scale,
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
   if (!actions.empty()) {
      auto last = actions.front();
      if (world->simulationTime >= last.time) {
         leftSpeed = last.vg;
         rightSpeed = last.vd;
         actions.pop();
      }
   }
   auto prediction = predict(widget, deltaTime);
   position = prediction->position;
   orientation = prediction->orientation;

   std::vector<Particule*> toDestroy;

   if (!hasTarget()) {
      setEvent(RobotEventState::NO_PARTICULE);
   }

   for (const auto& item: world->particules) {
      if (isInContactWithParticle(item)) {
         setEvent(RobotEventState::PARTICULE_CONTACT, item);
      }
      if (canAspirateParticle(
            item)) {//TODO: Dont know if a non-targeted particule can be aspirate...
         toDestroy.push_back(item);
      }
   }

   for (const auto& item: toDestroy){
      std::stringstream ss;
      ss << world->simulationTime << infos() << " " << item->infos();
      Logger::Log(LoggerType::P_COLLISION, ss.str());
      aspirate(item);
   }

}

bool Robot::collision(RobotData* a, RobotData* b) {
   double distance = a->position.getDistance(b->position);
   return distance <= a->robot->radius + b->robot->radius;
}

void Robot::stop() noexcept {
   if (leftSpeed + rightSpeed == 0) return;
   leftSpeed = 0;
   rightSpeed = 0;
   speedLog();
}

void Robot::emergencyStop() {
   if (!isEmergencyStopped)
      fl_beep(FL_BEEP_ERROR);

   setEvent(RobotEventState::COLLISION_WARNING);
   isEmergencyStopped = true;
   leftSpeed = rightSpeed = 0;
   speedLog();
   std::queue<RobotAction>().swap(actions); // clear actions;
}

bool Robot::addAction(double t, double vg, double vd) {
   if (!world) return false;

   if (t < 0 || (world && t < world->simulationTime)) {
      Logger::Log(LoggerType::WARNING, "Cant send a command in the past");
      return false;
   }
   if (!actions.empty() && t <= actions.back().time) {
      Logger::Log(LoggerType::WARNING,
                  "Need to send a command after the last");
      return false;
   }

   if (vg <= -world->constraint.vMin || vd <= -world->constraint.vMin) {
      Logger::Log(LoggerType::WARNING, "Error vMin");
      return false;
   }
   if ((vg + vd) / 2 >= world->constraint.vMax) {
      Logger::Log(LoggerType::WARNING, "Error vMax");
      return false;
   }
   if (!actions.empty()) {
      auto last = actions.back();
      if (abs(last.time - t) < world->constraint.dtMin) {
         Logger::Log(LoggerType::WARNING,
                     "Need to wait more");
         return false;
      }
      if (abs(vg - last.vg) >= world->constraint.dvMax ||
          abs(vd - last.vd) >= world->constraint.dvMax) {
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

bool Robot::isInContactWithParticle(Particule* particule) const {
   return position.getDistance(particule->getPosition()) <=
          radius + particule->getRadius();
}

bool Robot::canAspirateParticle(Particule* particule) const {
   return abs(getAlignementWithParticle(particule)) <= 1. &&
          isInContactWithParticle(particule);
}

double Robot::getAlignementWithParticle(Particule* particule) const {
   return norm_angle(position.getAngle(particule->getPosition()) - orientation);
}

std::tuple<double, double, double>
Robot::goToPosition(int speed, Position destination) {
   double a = to_rad(orientation);
   double distance = position.getDistance(destination);
   double R = (distance * distance / 2) /
              (cos(a) * (destination.y - position.y) -
               sin(a) * (destination.x - position.x));


   double vd = speed;
   double vg = ((R + radius) / (R - radius) * speed);
   { //Limit wheel speed
      limitWheelConstraint(vg, vd);
   }
   speedLog();

   double w = (vg - vd) / (2 * radius);
   double t = w == 0 ? distance / vg : (2.0 * asin((distance / 2.0) / R)) / w;
   double newAngle = to_deg(w * t);
   std::cout << "Robot go to " << destination.x << ":" << destination.y
             << ", arriving in " << t << " s with an angle of " << newAngle << "deg"
             << std::endl;
   return std::make_tuple(vg, vd, t);
}

std::tuple<double, double, double>
Robot::goToPositionDuration(double t, Position destination) {
   double a = to_rad(orientation);
   double distance = position.getDistance(destination);
   double R = (distance * distance / 2) /
              (cos(a) * (destination.y - position.y) -
               sin(a) * (destination.x - position.x));

   double w = (2.0 * asin((distance / 2.0) / R)) / t;
   double vt = w * R;
   double vd = (2 * vt) / ((R + radius) / (R - radius) + 1);
   double vg = (2 * vt) - vd;

   {//Limit wheel speed and recalculate time
      limitWheelConstraint(vg, vd);
      w = (vg - vd) / (2 * radius);
      t = w == 0 ? distance / vg : (2.0 * asin((distance / 2.0) / R)) / w;
   }
   speedLog();

   double newAngle = to_deg(w * t);
   std::cout << "Robot go to " << destination.x << ":" << destination.y
             << ", arriving in " << t << " s with an angle of " << newAngle << "deg"
             << std::endl;
   return std::make_tuple(vg, vd, t);
}

void Robot::rotate(double time, double newOrientation) {
   double now = world ? world->simulationTime : 0;
   if (newOrientation == orientation)return;
   double a = to_rad(norm_angle(newOrientation - orientation));
   double w = a / time;
   double vg = w * radius;
   double vd = -vg;
   limitWheelConstraint(vg, vd);
   std::cout << a << " " << vg << " " << -vg << std::endl;

   addAction(now, vg, vd);
   addAction(now + time, 0, 0);

   speedLog();
}

void Robot::limitWheelConstraint(double& vg, double& vd) {
   double ratio = vd / vg;
   if (vg < vd) {
      if (vg < -world->constraint.vMin) {
         vg = -world->constraint.vMin;
         vd = ratio * vg;
      }
   } else {
      if (vd < -world->constraint.vMin) {
         vd = -world->constraint.vMin;
         vg = vd / ratio;
      }
   }

   if ((vg + vd) / 2 >= world->constraint.vMax) {
      vg = (world->constraint.vMax * 2) / (1 + ratio);
      vd = ratio * vg;
   }
}

RobotEvent Robot::getEvent() const noexcept {
   return event;
}

void Robot::setEvent(RobotEventState newEvent, void* data) {
   if (event.event == newEvent ||
       (newEvent != RobotEventState::UNKNOWN && newEvent < event.event))
      return;
   std::cout << "[Event Robot#" << getId() << "] " << int(event.event) << " => "
             << int(newEvent)
             << std::endl;
   event.event = newEvent;
   event.data = data;
}

void Robot::aspirate(Particule* particule) {
   if (!particule)
      return;
   world->addCleanedEnergy(particule->getEnergy());
   std::stringstream ss;
   ss << world->simulationTime << " décontamination " << int(world->getCleanedEnergyRatio());
   Logger::Log(LoggerType::SCORE, ss.str());
   world->deleteParticule(particule);
}

void Robot::resetEvent() {
   setEvent(RobotEventState::UNKNOWN);
}

int Robot::getRadius() const noexcept {
   return radius;
}

bool operator<(const Robot& a, const Robot& b) {
   return a.getRadius() < b.getRadius();
}

void Robot::setTarget(Particule* target) {
   if (target == this->target)return;
   if (this->target != nullptr)this->target->removeTargeter(this);
   if (target != nullptr) {
      target->addTargeter(this);
      std::cout << "Target " << target->getId() << std::endl;
   }
   this->target = target;
}

Particule* Robot::getTarget() const noexcept {
   return target;
}

bool Robot::hasTarget() const noexcept {
   return target != nullptr;
}

double Robot::getLSpeed() const {
   return leftSpeed;
}

double Robot::getRSpeed() const {
   return rightSpeed;
}

std::string Robot::infos() const {
   std::stringstream ss;
   ss << " #" << getId() << " " << getPosition().x << " " << getPosition().y
      << " " << getRadius() << " " << to_rad(orientation)
      << " " << getLSpeed() << " " << getRSpeed();
   return ss.str();
}

void Robot::speedLog() const {
   std::stringstream ss;
   ss << world->simulationTime << infos();
   Logger::Log(LoggerType::R_VELOCITY, ss.str());
}

