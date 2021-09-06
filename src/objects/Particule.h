#ifndef PIN_2021_SERIE_1_PARTICULE_H
#define PIN_2021_SERIE_1_PARTICULE_H

#include <FL/Fl_Widget.H>
#include <vector>
#include "../core/Canvas.h"
#include "Object.h"

class Robot;

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
   double parentTime;
   double timeScale;
   int energy;
   std::vector<Robot*> targeter;

   unsigned getLifetimeUntilDeath(unsigned tn1, unsigned index) const;

public:
   Particule(
         Position position,
         int radius,
         int color,
         unsigned decomposeIndex,
         unsigned destroyIndex,
         unsigned tmaxe,
         double timeScale,
         int energy,
         double parentTime = 0,
         unsigned decomposeLevel = 1
   );

   ~Particule();

   unsigned getDecomposeLevel() const noexcept;

   std::vector<Particule*> createChilds() const;

   void update(WorldWidget* widget, double deltaTime) override;

   void draw(WorldWidget* widget) const override;

   double getDecomposeTime() const noexcept;

   double getDestroyTime() const noexcept;

   void explode();

   void init(unsigned id);

   static int getBaseEnergy(int radius, double scale) noexcept;

   int getRadius() const noexcept override;

   int getEnergy() const noexcept;

   void addTargeter(Robot* robot);

   void removeTargeter(Robot* robot);

   int getColor() const;

   std::string infos() const;
};


#endif //PIN_2021_SERIE_1_PARTICULE_H
