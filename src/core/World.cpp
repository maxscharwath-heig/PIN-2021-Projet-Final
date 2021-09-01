#include <iostream>
#include <vector>
#include <algorithm>
#include "World.h"

void World::clear() {
   simulationTime = 0;

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
      if (item->getDecomposeTime() <= simulationTime || item->getDestroyTime() <= simulationTime) {
         particulesToDelete.push_back(item);
      }
   }

   for(const auto& item: particulesToDelete){
      explode(item);
   }
}

void World::explode(Particule* particule) {
   particule->explode();
}

void World::addParticule(Particule* particule) {
   particule->setWorld(this);
   particules.push_back(particule);
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
