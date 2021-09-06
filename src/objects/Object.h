#ifndef PIN_2021_SERIE_1_OBJECT_H
#define PIN_2021_SERIE_1_OBJECT_H

#include <cmath>
#include "../utils/Utils.h"

class World;

class WorldWidget;

struct Position {
   double x = 0, y = 0;

   double getAngle(Position position) const {
      return to_deg(atan2(position.y - y, position.x - x));
   }

   double getDistance(Position position) const {
      double dx = position.x - x;
      double dy = position.y - y;
      return sqrt(dx * dx + dy * dy);
   }
};

class Object {
protected:
   Position position;
   World* world = nullptr;
   unsigned int id;
public:
   explicit Object(Position position);

   virtual void draw(WorldWidget* widget) const = 0;

   virtual void update(WorldWidget* widget, double deltaTime) = 0;

   Position getPosition();

   void setWorld(World* world);

   void init(unsigned id);

   unsigned int getId() const;

   virtual int getRadius() const = 0;
};


#endif //PIN_2021_SERIE_1_OBJECT_H
