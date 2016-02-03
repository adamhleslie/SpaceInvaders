#include "Player.h"

#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Projectile.h"
#endif

#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

class PlayerShip : public Entity, public Player
{
public:
    PlayerShip(int x, int y);
    ~PlayerShip();
    bool update(char input, int tick);
    bool lateUpdate(char input, int tick);

private:
    void parseInput(char input);
    void fireProjectile();
};