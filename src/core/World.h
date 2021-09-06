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
   int robotCounter;
   int totalEnergy;
   int cleanedEnergy;

public:
   void clear();

   void ready();

   void updateParticules();

   std::vector<Robot*> robots;
   std::vector<Particule*> particules;
   double simulationTime;
   Constraint constraint;

   void update(WorldWidget* widget, double deltaTime);

   void addParticule(Particule* particule);

   void deleteParticule(Particule* particule);

   void addRobot(Robot* robot);

   void deleteRobot(Robot* robot);

   int getCleanedEnergy() const noexcept;

   int getTotalEnergy() const noexcept;

   double getCleanedEnergyRatio() const noexcept;

   std::vector<Object*> getAllObjects() const;

   void addCleanedEnergy(int energy) noexcept;

   void resetEnergy() noexcept;

   ~World();
};

const size_t DECOMPOSE_LEVEL = 4;


#endif //PIN_2021_SERIE_1_WORLD_H
