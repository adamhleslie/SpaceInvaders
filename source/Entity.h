#include <list>
using std::list;

class SpaceInvadersController;

/**
    Anything that exists in the game world
*/
class Entity
{
public:
    Entity(int x = 0, int y = 0, bool drawable = false, char shape = '?');  // default constructor
    virtual ~Entity();
    static void (SpaceInvadersController::*setEndCondition)(int);                           // Function to update the end condition
    static void (SpaceInvadersController::*addEntity)(Entity* const);                       // Function to add an entity to the current game world
    static void (SpaceInvadersController::*removeEntity)(Entity* const);                    // Function to remove an entity from the current game world
    static list<Entity*>* (SpaceInvadersController::*getCollisions)(const Entity* const);   // Function to get all entities currently colliding with
    static SpaceInvadersController* game_controller;                                        // Pointer to game controller to use these functions on

    static int x_min;   // the min x displacement
    static int x_max;   // the max x displacement
    static int y_min;   // the min y displacement
    static int y_max;   // the max y displacement
    int x;              // value of x component of displacement with top left origin
    int y;              // value of y component of displacement with top left origin
    bool drawable;      // true if this should be drawn in any level present in
    char shape;         // the character representation of this entity

    virtual void earlyUpdate(char input, int tick); // Called every active tick, before any update is called
    virtual bool update(char input, int tick) = 0;  // Called every active tick. Returns true if to be destroyed
    virtual bool lateUpdate(char input, int tick);  // Called every active tick, after every update is called
};
