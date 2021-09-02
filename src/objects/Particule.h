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
   unsigned destroyIndex;
   unsigned decomposeLevel;
   unsigned tmaxe;
   double timeOfDestroy;
   double timeOfDecompose;
   const unsigned id;

   unsigned getLifetimeUntilDeath(unsigned tn1, unsigned index) const;


public:
   Particule(Position position, int radius, int color, unsigned decomposeIndex,
             unsigned destroyIndex, double currentTime, unsigned tmaxe, unsigned id, unsigned decomposeLevel = 1);

   unsigned getDecomposeLevel() const;

   std::vector<Particule*> createChilds(double currentTime) const;

   void update(WorldWidget* widget, double deltaTime) override;

   void draw(WorldWidget* widget) const override;

   double getDecomposeTime() const;

   double getDestroyTime() const;

   void explode();
};


#endif //PIN_2021_SERIE_1_PARTICULE_H
