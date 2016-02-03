#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Projectile.h"
#endif

#ifndef SQUADRONSHIP_H
#define SQUADRONSHIP_H
#include "SquadronShip.h"
#endif

#ifndef PLAYERSHIP_H
#define PLAYERSHIP
#include "PlayerShip.h"
#endif

Projectile::Projectile(int x, int y, float y_disp, bool killsEnemies, char shape) : 
                       Entity(x, y, true, shape),
                       y_float(y),
                       y_disp(y_disp),
                       killsEnemies(killsEnemies) {}

bool Projectile::update(char input, int tick)
{
    y_float += y_disp;
    y = (int) y_float;
    if(y > y_max || y < y_min)
        return true;

    return false;
}

bool Projectile::lateUpdate(char input, int tick)
{
    bool res = false;
    list<Entity*>* collisions = (game_controller->*getCollisions)(this);
    for (list<Entity*>::iterator iter = collisions->begin(); iter != collisions->end() && !res; iter++)
    {
        if((killsEnemies && dynamic_cast<SquadronShip*>(*iter)) || 
            (!killsEnemies && dynamic_cast<PlayerShip*>(*iter)))
        {
            (game_controller->*removeEntity)(*iter);

            res = true;
        }
    }
    delete collisions;

    return res;
}