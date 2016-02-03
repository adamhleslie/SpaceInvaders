#include <stdlib.h> 

#ifndef SQUADRONSHIP_H
#define SQUADRONSHIP_H
#include "SquadronShip.h"
#endif

char SquadronShip::shape_1;
char SquadronShip::shape_2;
int SquadronShip::count = 0;
int SquadronShip::x_dir = 1;
int SquadronShip::y_dir = 0;

/**
    Constructor places this ship at x, y as the next type
*/
SquadronShip::SquadronShip(int x, int y) : Entity(x, y, true)
{
    count++;
    if(count % 3 == 0)
    {
        shape = shape_1;
        fire_chance = kShape1FireChance;
        projectile = '*';
    } else {
        shape = shape_2;
        fire_chance = kShape2FireChance;
        projectile = '!';
    }
    moved_down = false;
}

/**
    Destructor reduces count, and checks if all killed
*/
SquadronShip::~SquadronShip()
{
    count--;
    if(count == 0)
        (game_controller->*setEndCondition)(1);
}

/**
    Decide how the ships will move during update
*/
void SquadronShip::earlyUpdate(char input, int tick)
{
    if (tick == 19)
    {
        if (y_dir == 0 && !moved_down)
        {
            if (x == x_max)
            {
                y_dir = 1;
                x_dir = -1;
            }
            else if (x == x_min)
            {
                y_dir = 1;
                x_dir = 1;
            }
        }
        else if (moved_down)
        {
            y_dir = 0;
        }
    }
}

/**
    Update moves squadron ships correctly, losing game
        if below the level
    Fires projectile based on fire_chance
*/
bool SquadronShip::update(char input, int tick)
{
    if (tick == 19)
    {
        if (y_dir != 0) {
            y += y_dir;
            if (y > y_max) 
            {
                (game_controller->*setEndCondition)(-1);
                return true;
            }
            moved_down = true;
        }
        else 
        {
            x += x_dir;
            moved_down = false;          
        }
    }

    if (fire_chance > ((float) rand()) / (RAND_MAX)) 
    {
        (game_controller->*addEntity)(new Projectile(x, y + 1, 0.5, false, projectile));
    }
    return false;
}