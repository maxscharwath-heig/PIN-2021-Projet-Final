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
      switch (robot->getEvent().event) {
         case RobotEventState::PARTICULE_CONTACT: {
            Particule* contact = (Particule*) robot->getEvent().data;
            if (robot->getTarget() == contact) {
               std::cout << "Contact with target" << std::endl;
            } else {
               robot->resetEvent();
            }
         }
            break;
         case RobotEventState::NO_PARTICULE:
            robot->setTarget(closestParticle(robot));
            robot->goToPositionDuration(10, robot->getTarget()->getPosition());
            robot->resetEvent();
            break;
         case RobotEventState::COLLISION_WARNING:
            //rotate or change direction
            //robot->resetEvent();
            break;
         default:
         case RobotEventState::UNKNOWN:
            break;
      }
   }
}
