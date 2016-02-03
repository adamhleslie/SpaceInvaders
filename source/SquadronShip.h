#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Projectile.h"
#endif

#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

class SquadronShip : public Entity
{
public:
    SquadronShip(int x, int y);         // Constructor to place in the world
    ~SquadronShip();                    // Default destructor
    void earlyUpdate(char input, int tick);
    bool update(char input, int tick);

    static char shape_1;    // Displayed representation of 1st ship type
    static char shape_2;    // Displayed representation of 2nd ship type

private:
    static const float kShape1FireChance = .0015;    // % fire chances
    static const float kShape2FireChance = .00075;
    static int count;       // The number of SquadronShips in existence
    static int x_dir;       // The current x direction heading in
    static int y_dir;       // The current y direction heading in
    char projectile;
    bool moved_down;        // True if moved down last movement
    float fire_chance;      // This instances fire chance
};