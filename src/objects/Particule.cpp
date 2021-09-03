#include "Particule.h"
#include <FL/FL_Draw.H>
#include <cmath>
#include <iostream>
#include "../core/World.h"
#include "../widgets/WorldWidget.h"

Particule::Particule(
      Position position,
      int radius,
      int color,
      unsigned decomposeIndex,
      unsigned destroyIndex,
      unsigned tmaxe,
      double timeScale,
      int energy,
      double parentTime,
      unsigned decomposeLevel
) :
      Object(position),
      radius(radius),
      color(color),
      decomposeIndex(decomposeIndex),
      destroyIndex(destroyIndex),
      decomposeLevel(decomposeLevel),
      tmaxe(tmaxe),
      parentTime(parentTime),
      timeScale(timeScale),
      energy(energy) {
}

void Particule::draw(WorldWidget* widget) const {
   fl_color(color);
   double scale = widget->scale->distance();
   double offsetX = widget->canvas->x();
   double offsetY = widget->canvas->y();
   int posX = (int) (widget->x() +
                     (position.x - radius + offsetX) * scale);
   int posY = (int) (widget->y() +
                     (position.y - radius + offsetY) * scale);
   int size = (int) (radius * 2.0 * scale);
   fl_pie(posX, posY, size, size, 0, 360);
}

std::vector<Particule*> Particule::createChilds() const {
   std::vector<Particule*> children;
   children.reserve(4);

   const int newRadius = radius / 2;
   const int newEnergy = energy / 2;

   children.emplace_back(
         new Particule({position.x + newRadius, position.y - newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex, tmaxe, timeScale, newEnergy, timeOfDecompose,
                       decomposeLevel + 1));
   children.emplace_back(
         new Particule({position.x + newRadius, position.y + newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex, tmaxe, timeScale, newEnergy, timeOfDecompose,
                       decomposeLevel + 1));
   children.emplace_back(
         new Particule({position.x - newRadius, position.y + newRadius},
                       newRadius,
                       color,
                       decomposeIndex,
                       destroyIndex,
                       tmaxe,
                       timeScale,
                       newEnergy,
                       timeOfDecompose,
                       decomposeLevel + 1));

   children.emplace_back(
         new Particule({position.x - newRadius, position.y - newRadius}, newRadius,
                       color,
                       decomposeIndex, destroyIndex, tmaxe, timeScale, newEnergy, timeOfDecompose,
                       decomposeLevel + 1));

   return children;
}

unsigned Particule::getDecomposeLevel() const {
   return decomposeLevel;
}

void Particule::explode() {
   if (!world) return;
   if (getDecomposeTime() <= getDestroyTime() &&
       getDecomposeLevel() <= DECOMPOSE_LEVEL) {
      for (Particule* part: createChilds()) {
         world->addParticule(part);
      }
   }
   world->deleteParticule(this);
}

unsigned Particule::getLifetimeUntilDeath(unsigned n, unsigned index) const {
   if (n <= 1) return index;
   return (getLifetimeUntilDeath(n - 1, index) * index) % tmaxe;
}

double Particule::getDecomposeTime() const {
   return timeOfDecompose;
}

double Particule::getDestroyTime() const {
   return timeOfDestroy;
}

void Particule::update(WorldWidget* widget, double deltaTime) {
   // TODO: Call explode from here ?

}

void Particule::init(unsigned id) {
   this->id = id;
   timeOfDestroy = parentTime + getLifetimeUntilDeath(id, destroyIndex) * timeScale;
   timeOfDecompose = parentTime + getLifetimeUntilDeath(id, decomposeIndex) * timeScale;
}

int Particule::getBaseEnergy(int radius, double scale) {
   return (int)(M_PI * pow(radius * scale, 2));
}

int Particule::getEnergy() const {
   return energy;
}

int Particule::getRadius() const {
   return radius;
}
