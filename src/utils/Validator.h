#ifndef PIN_2021_SERIE_2_VALIDATOR_H
#define PIN_2021_SERIE_2_VALIDATOR_H


#include <vector>
#include "../objects/Object.h"
#include "../objects/Robot.h"

class Validator
{
public:
   static bool itemsOverlapping(const std::vector<Object*>& objects);
   static bool robotsSizesValids(const std::vector<Robot*>& robots);
   static bool robotsAndParticulesSizesValids(const std::vector<Robot*>& robots, const std::vector<Particule*>& particules);

private:
   static bool areOverlapping(Object *a, Object *b);
};


#endif //PIN_2021_SERIE_2_VALIDATOR_H
