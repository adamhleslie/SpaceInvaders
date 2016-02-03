#include <ncurses.h>    // ncurses library
#include <stdlib.h>     // srand, rand, and printf (cannout use cout/cin while using ncurses)
#include <unistd.h>     // sleep
#include <time.h>       // time
#include <list>         // standard library list

#include "SpaceInvadersController.h"    // Header File
#include "timehelper.h"
// #include "MotherShip.h"

using std::list;

/**
    Default Constructor, references this controller to all entities
*/
SpaceInvadersController::SpaceInvadersController(): end_condition(0), 
                                                    max_player_projectiles(5), 
                                                    cur_player_projectiles(5),
                                                    points(0)
{
    // give entites access to important functions
    Entity::game_controller = this;
    Entity::setEndCondition = &SpaceInvadersController::setEndCondition;
    Entity::addEntity = &SpaceInvadersController::addEntity;
    Entity::removeEntity = &SpaceInvadersController::removeEntity;
    Entity::getCollisions = &SpaceInvadersController::getCollisions;

    srand(time(NULL)); // seed our random number generator
}

/**
    Destructor deletes all active entities, and removes external refernces to this instance
*/
SpaceInvadersController::~SpaceInvadersController()
{
    for (list<Entity*>::iterator iter = live_entities.begin(); iter != live_entities.end(); iter++)
        delete *iter;

    Entity::game_controller = NULL;
    Entity::setEndCondition = NULL;
    Entity::addEntity = NULL;
    Entity::removeEntity = NULL;
    Entity::getCollisions = NULL;
}

/**
    Creates the ncurses screen and sets the screen settings
*/
void SpaceInvadersController::createScreen()
{
    // init the screen, defaulting to stdscr
    if (initscr() == NULL) { 
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho();               // don't echo keypresses
    keypad(stdscr, TRUE);   // set the keypad to stdscr
    nodelay(stdscr, false); // getch waits until a key is pressed instead of returning ERR (?)
    curs_set(0);            // set cursor to invisible (if allowed by the os)
    timeout(kTickDelay);    // set how long between keypresses (in milliseconds)
}

/**
    Destroys the ncurses screen
*/
int SpaceInvadersController::destroyScreen()
{
    // cleanup the window
    endwin();
    return points;
}

/**
    Creates a level with the specified sizes and squadron shapes
*/
void SpaceInvadersController::createLevel(int x_size, int y_size, 
                                          char squadron_shape_1, char squadron_shape_2)
{
    Entity::x_min = kXOffset;
    Entity::x_max = x_size + kXOffset;
    Entity::y_min = kYOffset;
    Entity::y_max = y_size + kYOffset;

    x_navigable_size = x_size;
    y_navigable_size = y_size;

    SquadronShip::shape_1 = 'W';
    SquadronShip::shape_2 = 'u';
    // populate level with SquadronShips
    int max_y = kYOffset + (y_size / 2) - kEnemySquadronYEdgeDist;
    int max_x = kXOffset + x_size - kEnemySquadronXEdgeDist;
    for (int y = kYOffset + kEnemySquadronYEdgeDist; y < max_y; y += kEnemySquadronYInnerDisp)
    {
        for (int x = kXOffset + kEnemySquadronXEdgeDist; x < max_x; x += kEnemySquadronXInnerDisp)
        {
            addEntity(new SquadronShip(x, y));
        } 
    }

    addEntity(new PlayerShip((int) (x_size / 2) + kXOffset, y_size + kYOffset));
}

/**
    Begins and maintains the game loop till an end condition met
*/
int SpaceInvadersController::run()
{
    int elapsedTime = getElapsedTime();
    int cur_tick = 0, cur_input = 0;

    // Game Loop: until end condition met (-2 if quit, -1 if lost, 1 if won)
    while (end_condition == 0)
    {
        cur_tick = (cur_tick + 1) % kTicksPerSec;
        cur_input = getch();
        erase(); // erase the screen after getch()

        // wait till the next tick if input given
        if (cur_input != ERR) {
            elapsedTime = getElapsedTime();
            if (elapsedTime < kTickDelay)
                nsleep(kTickDelay - elapsedTime);
        }

        // see if player quit, or changed a setting mid game
        evalInput(cur_input);

        // draw the level before any entities
        drawLevel();

        Entity* p_cur_entity;
        // loop through all active entities and early update each for this tick
        for (list<Entity*>::iterator iter = live_entities.begin(); iter != live_entities.end(); iter++)
        {
            p_cur_entity = *iter;
            p_cur_entity->earlyUpdate(cur_input, cur_tick);
        }

        // loop through all active entities and update each for this tick, destroying them or not
        for (list<Entity*>::iterator iter = live_entities.begin(); iter != live_entities.end(); iter++)
        {
            p_cur_entity = *iter;
            // Update the current entity
            if (p_cur_entity->update(cur_input, cur_tick))
            {
                // Destroy the current entity if update returned true
                delete p_cur_entity;
                iter = live_entities.erase(iter);
            } 
        }

        // loop through all active entities and late update each for this tick, destroying or drawing them
        for (list<Entity*>::iterator iter = live_entities.begin(); iter != live_entities.end(); iter++)
        {
            p_cur_entity = *iter;
            if (p_cur_entity->lateUpdate(cur_input, cur_tick))
            {
                // Destroy the current entity if update returned true
                delete p_cur_entity;
                iter = live_entities.erase(iter);
            } 
            else if (p_cur_entity->drawable)
            {
                // Draw the current entity
                mvaddch(p_cur_entity->y, p_cur_entity->x, p_cur_entity->shape);
            }
        }

        // refresh the screen and hide cursor after drawing everything
        refresh();
        move(0,0);

        updateEndCondition();
    }

    return end_condition;
}

void SpaceInvadersController::evalInput(char input)
{
    if(input == 'r')
        cur_player_projectiles = max_player_projectiles;
    else if (input == 'q')
        setEndCondition(-2);
    else if ('0' <= input && input <= '9')
        max_player_projectiles = input - '0';
}

/**
    Adds game specific drawings 
    Note: You should make these affect where live entities are drawn in the game loop
*/
void SpaceInvadersController::drawLevel()
{
    mvprintw(0, kXOffset - 4, "Points: %i    Cur Ammo: %i    Max Ammo: %i\n", points, cur_player_projectiles, max_player_projectiles);
    mvprintw(y_navigable_size + kYOffset + 2, kXOffset - 18, "'q' quit, 'a'/'d' move, 'w' shoot, 'r' reload, '0'-'9' change Max Ammo");
    drawRectangle(kXOffset - 1, kYOffset - 1, x_navigable_size + 2, y_navigable_size + 2, '-', '|');
}

void SpaceInvadersController::drawRectangle(int x, int y, int x_length, int y_length, char horiz, char vert)
{
    drawLine(x, y, x, y + y_length, 0, 1, vert);
    drawLine(x + x_length, y, x + x_length, y + y_length, 0, 1, vert);
    drawLine(x, y, x + x_length, y, 1, 0, horiz);
    drawLine(x, y + y_length, x + x_length, y + y_length, 1, 0, horiz);
}

void SpaceInvadersController::drawLine(int x, int y, int x2, int y2, int x_change, int y_change, char shape)
{
    while(x != x2 || y != y2)
    {
        mvaddch(y, x, shape);
        x += x_change;
        y += y_change;
    }

    mvaddch(y2, x2, shape);
}

/**
    Adds an entity to the active entities
*/
void SpaceInvadersController::addEntity(Entity* const new_entity)
{
    Projectile* proj = dynamic_cast<Projectile*>(new_entity);
    if (proj && proj->killsEnemies && cur_player_projectiles > 0)
    {
        cur_player_projectiles--;
        live_entities.push_back(new_entity);
    } 
    else if (!proj || (proj && !proj->killsEnemies))
        live_entities.push_back(new_entity);
}

/**
    Removes an entity from the active entities, and destroys it
*/
void SpaceInvadersController::removeEntity(Entity* const dead_entity)
{
    if (dynamic_cast<SquadronShip*>(dead_entity))
        points++;
    live_entities.remove(dead_entity);
    delete dead_entity;
}

/**
    Returns a reference to a list of all the colliding active elements
*/
list<Entity*>* SpaceInvadersController::getCollisions(const Entity* const collided)
{
    list<Entity*>* collided_with = new list<Entity*>();
    for (list<Entity*>::iterator iter = live_entities.begin(); iter != live_entities.end(); iter++)
    {
        if ((*iter)->x == collided->x && (*iter)->y == collided->y)
            collided_with->push_back(*iter);
    }

    return collided_with;
}

/**
    Sets the end condition to val
    pre: val = -2 if quit, -1 if lose, 1 if win
*/
void SpaceInvadersController::setEndCondition(int val)
{
    if (end_condition == 0)
        end_condition = val;
}

/**
    Updates the end condition based on things not checked by entities
*/
void SpaceInvadersController::updateEndCondition()
{
    if (end_condition == 0)
    {
        // check end conditions
    }
}