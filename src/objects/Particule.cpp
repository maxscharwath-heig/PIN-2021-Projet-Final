#include "Particule.h"
#include <FL/FL_Draw.H>
#include "../core/World.h"
#include "../widgets/WorldWidget.h"

Particule::Particule(
      Position position,
      int radius,
      int color,
      unsigned decomposeIndex,
      unsigned destroyIndex,
      double currentTime,
      unsigned tmaxe,
      const unsigned id,
      unsigned decomposeLevel) :
      Object(position),
      radius(radius),
      color(color),
      decomposeIndex(decomposeIndex),
      destroyIndex(destroyIndex),
      decomposeLevel(decomposeLevel),
      tmaxe(tmaxe),
      id(id)
      {
         timeOfDestroy = currentTime + getLifetimeUntilDeath(id, destroyIndex);
         timeOfDecompose = currentTime + getLifetimeUntilDeath(id, decomposeIndex);
      }

void Particule::draw(WorldWidget* widget) const {
   fl_color(color);
   double ratio = widget->scale->getScaleRatio();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();
   int posX = (int) (widget->x() + (position.x - radius + offsetX) * ratio);
   int posY = (int) (widget->y() + (position.y - radius + offsetY) * ratio);
   int size = (int) (radius * 2.0 * ratio);
   fl_pie(posX, posY, size, size, 0, 360);
}

std::vector<Particule*> Particule::createChilds(double currentTime) const {
   std::vector<Particule*> children;
   children.reserve(4);

   const int newRadius = radius / 2;

   children.emplace_back(
         new Particule({position.x - newRadius, position.y - newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex,
                       currentTime, tmaxe, id + 1, decomposeLevel + 1));
   children.emplace_back(
         new Particule({position.x + newRadius, position.y - newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex,
                       currentTime, tmaxe,id + 2,decomposeLevel + 1));
   children.emplace_back(
         new Particule({position.x - newRadius, position.y + newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex,
                       currentTime, tmaxe, id + 3,decomposeLevel + 1));
   children.emplace_back(
         new Particule({position.x + newRadius, position.y + newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex,
                       currentTime, tmaxe,id + 4,decomposeLevel + 1));
   return children;
}

unsigned Particule::getDecomposeIndex() const {
   return decomposeIndex;
}

unsigned Particule::getDestroyIndex() const {
   return destroyIndex;
}

unsigned Particule::getDecomposeLevel() const {
   return decomposeLevel;
}

void Particule::explode() {
   if (!world) return;
   if (getDecomposeTime() <= getDestroyTime() && getDecomposeLevel() <= DECOMPOSE_LEVEL) {
      for (Particule* part: createChilds(world->simulationTime)) {
         world->addParticule(part);
      }
   }
   world->deleteParticule(this);
}

unsigned Particule::getLifetimeUntilDeath(unsigned n, unsigned index) const
{
   if (n <= 1) return index;
   return (getLifetimeUntilDeath(n-1, index) * index) % tmaxe;
}

double Particule::getDecomposeTime() const
{
   return timeOfDecompose;
}

double Particule::getDestroyTime() const
{
   return timeOfDestroy;
}
