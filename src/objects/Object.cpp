#include "Object.h"
#include "../core/World.h"
#include "../widgets/WorldWidget.h"

void Object::setWorld(World* world) {
   this->world = world;
}

Position Object::getPosition() {
   return position;
}

Object::Object(Position position) {
   this->position = position;
}
