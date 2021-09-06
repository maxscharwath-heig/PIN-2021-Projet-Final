#include <iostream>
#include <tuple>
#include "Coordinator.h"

Coordinator::Coordinator(World* world) : world(world) {}

Coordinator::~Coordinator() {
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

            if (contact && robot->getTarget() == contact) {
               robot->resetEvent();
            } else {
               robot->resetEvent();
            }
            break;
         }

         case RobotEventState::NO_PARTICULE: {
            auto target = closestParticle(robot);
            robot->setTarget(target);

            //robot->goToPosition(100, target->getPosition());

            // auto tuple = robot->goToPositionDuration(10, target->getPosition());//0gauche 1droite 2temps pour y arriver
            robot->goToPosition(30, target->getPosition());

            // robot->addAction(world->simulationTime, std::get<0>(tuple), std::get<1>(tuple));
            robot->resetEvent();
            break;
         }

         case RobotEventState::COLLISION_WARNING:
            // rotate or change direction
            //robot->rotate(world->simulationTime, 360);
            robot->resetEvent();
            break;
         default:
         case RobotEventState::UNKNOWN:
            break;
      }
   }
}
