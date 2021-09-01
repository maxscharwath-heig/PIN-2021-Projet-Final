#ifndef PIN_2021_SERIE_1_PARTICULE_H
#define PIN_2021_SERIE_1_PARTICULE_H

#include <FL/Fl_Widget.H>
#include <vector>
#include "../core/Canvas.h"
#include "Object.h"

class Particule : public Object {
private:
   const int radius;
   int color;
   unsigned decomposeIndex;
   const unsigned parentDecomposeIndex;
   unsigned destroyIndex;
   const unsigned parentDestroyIndex;
   unsigned decomposeLevel;
   unsigned tmaxe;
   unsigned parentLife;
   double timeOfDestroy;
   double timeOfDecompose;
   const unsigned id;

   unsigned getLifetimeUntilDeath(unsigned tn1, unsigned index) const;


public:
   Particule(Position position, int radius, int color, unsigned decomposeIndex,
             unsigned destroyIndex, double currentTime, unsigned tmaxe, unsigned id, unsigned decomposeLevel = 1);

   unsigned getDecomposeIndex() const;

   unsigned getDestroyIndex() const;

   unsigned getDecomposeLevel() const;

   std::vector<Particule*> createChilds(double currentTime) const;

   void decreaseIndexes(size_t diff = 1);

   void draw(WorldWidget* widget) const;

   double getDecomposeTime() const;

   double getDestroyTime() const;


   void explode();
};


#endif //PIN_2021_SERIE_1_PARTICULE_H
