#include "Entity.h"

// make memory for our static variables
void (SpaceInvadersController::*Entity::setEndCondition)(int);
void (SpaceInvadersController::*Entity::addEntity)(Entity* const); 
void (SpaceInvadersController::*Entity::removeEntity)(Entity* const); 
list<Entity*>* (SpaceInvadersController::*Entity::getCollisions)(const Entity* const); 
SpaceInvadersController* Entity::game_controller;

int Entity::x_min;
int Entity::x_max;
int Entity::y_min;
int Entity::y_max;

Entity::Entity(int x, int y, bool drawable, char shape): 
               x(x), y(y), drawable(drawable), shape(shape) {}

Entity::~Entity() {}

void Entity::earlyUpdate(char input, int tick) {}

bool Entity::lateUpdate(char input, int tick) { return false; }