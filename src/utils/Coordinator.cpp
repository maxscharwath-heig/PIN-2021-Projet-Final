#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator(World* world) : world(world) { }

Coordinator::~Coordinator()
{
   world = nullptr;
}

Particule* Coordinator::closestParticle(Robot* robot) {
   Particule* closest = nullptr;
   for (const auto& particule: world->particules) {
      if (closest == nullptr ||
          robot->getPosition().getDistance(particule->getPosition()) <
          robot->getPosition().getDistance(closest->getPosition())) {
         closest = particule;
      }
   }
   return closest;
}

void Coordinator::update() {
   for (const auto& robot: world->robots) {
      switch (robot->getEvent()) {
         case RobotEvent::PARTICULE_CONTACT:
            //need to orientate to right direction (check if is targeted ?)
            robot->resetEvent();
            break;
         case RobotEvent::NO_PARTICULE:
            robot->setTarget(closestParticle(robot));
            robot->goToPositionDuration(10, robot->getTarget()->getPosition());
            robot->resetEvent();
            break;
         case RobotEvent::COLLISION_WARNING:
            //rotate or change direction
            //robot->resetEvent();
            break;
         default:
         case RobotEvent::UNKNOWN:
            break;
      }
   }
}
