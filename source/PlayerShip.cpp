#ifndef SQUADRONSHIP_H
#define SQUADRONSHIP_H
#include "SquadronShip.h"
#endif

#ifndef PLAYERSHIP_H
#define PLAYERSHIP
#include "PlayerShip.h"
#endif

#include <ncurses.h>
#include <list>

PlayerShip::PlayerShip(int x, int y) : Entity(x, y, true, '^') {}

PlayerShip::~PlayerShip()
{
    (game_controller->*setEndCondition)(-1);
}

bool PlayerShip::update(char input, int tick)
{
    parseInput(input);
    if (x < x_min)
        x++;
    else if (x > x_max)
        x--;

    return false;
}

bool PlayerShip::lateUpdate(char input, int tick)
{
    list<Entity*>* collisions = (game_controller->*getCollisions)(this);
    for (list<Entity*>::iterator iter = collisions->begin(); iter != collisions->end(); iter++)
    {
        if(dynamic_cast<SquadronShip*>(*iter))
            (game_controller->*setEndCondition)(-1);
    }
    delete collisions;

    return false;
}

void PlayerShip::parseInput(char input)
{
    switch (input)
    {
        case 'a': x--; break;
        case 'd': x++; break;
        case 'w':
            fireProjectile();
            break;
    }
}

void PlayerShip::fireProjectile()
{
    (game_controller->*addEntity)(new Projectile(x, y - 1, -1, true, '|'));
}