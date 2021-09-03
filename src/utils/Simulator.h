#ifndef PIN_2021_SERIE_2_SIMULATOR_H
#define PIN_2021_SERIE_2_SIMULATOR_H

#include "../objects/Robot.h"

class Simulator {
   Simulator();

   void giveOrder(Robot*);

   unsigned getScore();

};


#endif //PIN_2021_SERIE_2_SIMULATOR_H
