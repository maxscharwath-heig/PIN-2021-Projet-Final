#include <iostream>
#include <vector>
#include <algorithm>
#include "World.h"

void World::clear() {
   simulationTime = 0;
   particleCounter = 0;

   for (const auto& item: robots) {
      delete item;
   }
   robots.clear();

   for (const auto& item: particules) {
      delete item;
   }
   particules.clear();
}

void World::updateParticules() {
   std::vector<Particule*> particulesToDelete;

   for (const auto& item: particules) {
      if (item->getDecomposeTime() <= simulationTime ||
          item->getDestroyTime() <= simulationTime) {
         particulesToDelete.push_back(item);
      }
   }

   for (const auto& item: particulesToDelete) {
      explode(item);
   }
}

void World::explode(Particule* particule) {
   particule->explode();
}

void World::addParticule(Particule* particule) {
   particule->init(++particleCounter);
   particule->setWorld(this);
   particules.push_back(particule);

   totalEnergy += particule->getEnergy();
}

void World::deleteParticule(Particule* particule) {
   auto el = std::find(particules.begin(), particules.end(), particule);
   if (el == particules.end()) return;
   particules.erase(el);
   delete particule;
}

void World::addRobot(Robot* robot) {
   robot->setWorld(this);
   robots.push_back(robot);
}

void World::deleteRobot(Robot* robot) {
   auto el = std::find(robots.begin(), robots.end(), robot);
   if (el == robots.end()) return;
   robots.erase(el);
   delete robot;
}

void World::update(WorldWidget* widget, double deltaTime) {
   simulationTime += deltaTime;
   std::vector<RobotData*> robotsPredicted;
   for (const auto& item: robots) {
      robotsPredicted.push_back(item->predict(widget, deltaTime * 1.5));
   }
   for (const auto& a: robotsPredicted) {
      a->robot->collided = false;
      for (const auto& b: robotsPredicted) {
         if (a == b)continue;
         if (Robot::collision(a, b)) {
            a->robot->collided = true;
            //a->robot->stop();
            break;
         }
      }
   }
   for (const auto& item: robots) {
      item->update(widget, deltaTime);
   }
   updateParticules();
}

int World::getTotalEnergy() const {
   return totalEnergy;
}

int World::getCleanedEnergy() const {
   return cleanedEnergy;
}

void World::ready() {
   for (const auto& item: robots) {
      item->goToPositionDuration(10, particules.front()->getPosition());
   }
}
