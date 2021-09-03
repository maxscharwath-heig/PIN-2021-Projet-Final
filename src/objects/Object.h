#ifndef PIN_2021_SERIE_1_OBJECT_H
#define PIN_2021_SERIE_1_OBJECT_H

#include <cmath>

class World;

class WorldWidget;

struct Position {
   double x = 0, y = 0;

   double getAngle(Position position) {
      return atan2(position.y - y, position.x - x) * 180 / M_PI;
   }

   double getDistance(Position position) {
      double dx = position.x - x;
      double dy = position.y - y;
      return sqrt(dx * dx + dy * dy);
   }
};

class Object {
protected:
   Position position;
   World* world = nullptr;
public:
   explicit Object(Position position);

   virtual void draw(WorldWidget* widget) const = 0;

   virtual void update(WorldWidget* widget, double deltaTime) = 0;

   Position getPosition();

   void setWorld(World* world);
};


#endif //PIN_2021_SERIE_1_OBJECT_H
