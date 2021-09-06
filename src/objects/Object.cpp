#include <iostream>
#include "Object.h"
#include "../core/World.h"
#include "../widgets/WorldWidget.h"

Object::Object(Position position) {
   this->position = position;
}

void Object::setWorld(World* world) {
   this->world = world;
}

Position Object::getPosition() const noexcept {
   return position;
}

void Object::init(unsigned int id) {
   this->id = id;
}

unsigned int Object::getId() const noexcept{
   return id;
}
