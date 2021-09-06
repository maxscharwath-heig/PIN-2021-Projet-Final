#ifndef PIN_2021_SERIE_2_COORDINATOR_H
#define PIN_2021_SERIE_2_COORDINATOR_H

#include "../objects/Robot.h"
#include "../core/World.h"

class Coordinator {
private:
   World* world;
public:
   Coordinator(World* world);

   void update();
};


#endif //PIN_2021_SERIE_2_COORDINATOR_H
