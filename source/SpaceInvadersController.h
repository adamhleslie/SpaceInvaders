#include <list>

#ifndef SQUADRONSHIP_H
#define SQUADRONSHIP_H
#include "SquadronShip.h"
#endif

#ifndef PLAYERSHIP_H
#define PLAYERSHIP
#include "PlayerShip.h"
#endif

using std::list;

/**
    Game Controller controls the integral game loop for Space Invaders
    You should manipulate the game controller outside of the game loop to add functionality
        such as menus, levels, and cutscenes
*/
class SpaceInvadersController
{
public:
    SpaceInvadersController();                          // Default Constructor
    ~SpaceInvadersController();                         // Default Destructor

    void createScreen();                                // Creates a new ncurses screen
    int destroyScreen();                               // Destroys the active ncurses screen and window
    void createLevel(int x_size, int y_size,            // Creates a level with specified size and enemy settings
                    char squadron_shape_1 = 'W', 
                    char squadron_shape_2 = 'u');        
    int run();                                          // Runs the game loop. Returns the end condition
    void drawLevel();                                   // Draws game specific console features
    void addEntity(Entity* const);                      // Makes an entity active in the level
    void removeEntity(Entity* const);                   // Makes and entity inactive in the level
    list<Entity*>* getCollisions(const Entity* const);  // Returns the entites colliding with
    void setEndCondition(int);                          // Setter for the end condition

private:
    static const int kTickDelay = 50;                   // The number of milliseconds per tick
    static const int kTicksPerSec = 20;                 // The number of tics per second
    static const int kXOffset = 20;                     // The x displacement for use by drawLevel()
    static const int kYOffset = 2;                      // The y displacement for use by drawLevel()
    static const int kEnemySquadronXEdgeDist = 4;       // Displacements for spawning SquadronShips
    static const int kEnemySquadronYEdgeDist = 3;
    static const int kEnemySquadronXInnerDisp = 2;
    static const int kEnemySquadronYInnerDisp = 2;

    list<Entity*> live_entities;    // The entities active in the game world
    int x_navigable_size;           // The x size of the current navigable area
    int y_navigable_size;           // The y size of the current navigable area
    int end_condition;              // -2 if manual quit, -1 if game has been lost, 0 if still playing, 1 if won
    int max_player_projectiles;     // The maximum number of player projectiles at once
    int cur_player_projectiles;     // The number of player projectiles present
    int points;                     // The number of points the player has

    void evalInput(char input);     // Updates the state based input
    void updateEndCondition();      // Updates the end_condition based on state
    void drawRectangle(int x, int y, int x_length, int y_length, char horiz, char vert);
    void drawLine(int x1, int y1, int x2, int y2, int x_change, int y_change, char shape);
};