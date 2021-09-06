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
   return distance <= (b->getRadius() + a->getRadius()) and distance >= abs(b->getRadius() - a->getRadius());
}

bool Validator::robotsSizes(const std::vector<Robot *>& robots)
{
   return false;
}
