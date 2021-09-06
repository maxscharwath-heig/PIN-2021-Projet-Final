#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator(World* world) : world(world) {
}

void Coordinator::update() {
   for (const auto& robot: world->robots) {
      switch (robot->getEvent()) {
         case RobotEvent::PARTICULE_CONTACT:
            //need to orientate to right direction (check if is targeted ?)
            robot->resetEvent();
            break;
         case RobotEvent::NO_PARTICULE:
            //find closest particule and add to target
            robot->resetEvent();
            break;
         case RobotEvent::COLLISION_WARNING:
            //rotate or change direction
            robot->resetEvent();
            break;
         default:
         case RobotEvent::UNKNOWN:
            break;
      }
   }
}
