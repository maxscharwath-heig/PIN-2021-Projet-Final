#ifndef PIN_2021_SERIE_1_WORLD_H
#define PIN_2021_SERIE_1_WORLD_H

#include <vector>
#include "../objects/Robot.h"
#include "../objects/Particule.h"
#include "Canvas.h"

struct Constraint {
   int vMin = 0;
   int vMax = 0;
   int dtMin = 0;
   int dvMax = 0;
};

class World {
private:
   int particleCounter;
   int totalEnergy;
   int cleanedEnergy;

public:
   void clear();

   void ready();

   void updateParticules();

   void explode(Particule* particule);

   std::vector<Robot*> robots;
   std::vector<Particule*> particules;
   double simulationTime;
   Constraint constraint;

   void update(WorldWidget* widget, double deltaTime);

   void addParticule(Particule* particule);

   void deleteParticule(Particule* particule);

   void addRobot(Robot* robot);

   void deleteRobot(Robot* robot);

   int getCleanedEnergy() const;

   int getTotalEnergy() const;
};

const size_t DECOMPOSE_LEVEL = 4;


#endif //PIN_2021_SERIE_1_WORLD_H
