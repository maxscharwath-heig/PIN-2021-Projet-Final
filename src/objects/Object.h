//
// Created by Maxime on 31.08.2021.
//

#ifndef PIN_2021_SERIE_1_OBJECT_H
#define PIN_2021_SERIE_1_OBJECT_H

class World;

class WorldWidget;

struct Position {
   double x = 0, y = 0;
};

class Object {
protected:
   Position position;
   World* world = nullptr;
public:
   explicit Object(Position position);

   virtual void draw(WorldWidget* widget) const;

   virtual void update(WorldWidget* widget, double deltaTime);

   Position getPosition();

   void setWorld(World* world);
};


#endif //PIN_2021_SERIE_1_OBJECT_H
