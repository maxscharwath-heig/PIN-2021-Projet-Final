#include <iostream>
#include <algorithm>
#include "Validator.h"

bool Validator::itemsOverlapping(const std::vector<Object *>& objects)
{
   for (const auto* a : objects) {
      for (const auto* b : objects) {
         if (a == b) continue;
         if (areOverlapping((Object *) a, (Object *)b))
            return true;
      }
   }
   return false;
}

bool Validator::areOverlapping(Object *a, Object *b)
{
   double distance = sqrt(pow(a->getPosition().x - b->getPosition().x, 2) + (pow(a->getPosition().y - b->getPosition().y, 2)));
   return distance < (b->getRadius() + a->getRadius()) && distance >= abs(b->getRadius() - a->getRadius());
}

bool Validator::robotsSizesValids(const std::vector<Robot *>& robots) {
   for (const auto* a : robots) {
      for (const auto* b : robots) {
         if (a == b) continue;
         return std::max(a->getRadius(), b->getRadius()) <= std::min(a->getRadius(), b->getRadius() * 2);
      }
   }
   return true;
}

bool Validator::robotsAndParticulesSizesValids(const std::vector<Robot *>& robots,
                                               const std::vector<Particule *>& particules) {

   if (robots.empty() || particules.empty()) return true;

   auto smallest = std::min_element(robots.begin(), robots.end());
   auto biggest = std::min_element(robots.begin(), robots.end());

   return std::all_of(particules.begin(), particules.end(), [smallest, biggest](Particule* p) {
      return (*smallest)->getRadius() <= p->getRadius() && (p->getRadius() / 16) <= (*biggest)->getRadius();
   });
}
