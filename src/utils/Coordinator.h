#ifndef PIN_2021_SERIE_2_COORDINATOR_H
#define PIN_2021_SERIE_2_COORDINATOR_H

#include "../objects/Robot.h"
#include "../core/World.h"

class Coordinator {
private:
   World* world;

   Particule* closestParticle(Robot* robot);

public:
   explicit Coordinator(World* world);
   ~Coordinator();

   void update();
};


#endif //PIN_2021_SERIE_2_COORDINATOR_H
