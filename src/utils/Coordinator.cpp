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
            robot->resetEvent();
            break;
         }

         case RobotEventState::COLLISION_WARNING:
            robot->rotate(30);
            robot->resetEvent();
            break;
         default:
         case RobotEventState::UNKNOWN:
            if (robot->hasTarget()) {
               auto target = robot->getTarget();
               double angle = robot->getAlignementWithParticle(target);
               if (std::abs(angle) > 90) {
                  if (!robot->isRotating())
                     robot->rotate(angle > 0 ? 30 : -30);
               } else {
                  auto tuple = robot->goToPositionDuration(1, target->getPosition());
                  robot->addAction(world->simulationTime, std::get<0>(tuple),
                                   std::get<1>(tuple));
               }
            }
            break;
      }
   }
}
