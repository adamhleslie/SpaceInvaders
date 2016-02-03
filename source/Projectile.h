#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

class Projectile : public Entity
{
public:
    Projectile(int x, int y, float y_disp, bool killsEnemies, char shape);
    bool update(char input, int tick);
    bool lateUpdate(char input, int tick);

    float y_float;
    float y_disp;
    bool killsEnemies;
};