#ifndef PIN_2021_SERIE_1_WORLD_H
#define PIN_2021_SERIE_1_WORLD_H

#include <vector>
#include "../objects/Robot.h"
#include "../objects/Particule.h"
#include "Canvas.h"

class World {
public:
   void clear();

   void updateParticules();

   void explode(Particule* particule);

   std::vector<Robot*> robots;
   std::vector<Particule*> particules;
   double simulationTime;

   void addParticule(Particule* particule);

   void deleteParticule(Particule* particule);

   void addRobot(Robot* robot);

   void deleteRobot(Robot* robot);
};

const size_t DECOMPOSE_LEVEL = 4;


#endif //PIN_2021_SERIE_1_WORLD_H
