#ifndef PIN_2021_SERIE_2_VALIDATOR_H
#define PIN_2021_SERIE_2_VALIDATOR_H


#include <vector>
#include "../objects/Object.h"
#include "../objects/Robot.h"

class Validator
{
public:
   static bool itemsOverlapping(const std::vector<Object*>& objects);
   static bool robotsSizes(const std::vector<Robot*>& robots);


private:
   static bool areOverlapping(Object *a, Object *b);

};


#endif //PIN_2021_SERIE_2_VALIDATOR_H
