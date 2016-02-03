#include <iostream>
#include "SpaceInvadersController.h"

int main(int argc, char const *argv[])
{
    SpaceInvadersController game_controller;

    game_controller.createScreen();
    game_controller.createLevel(30, 35);
    int result = game_controller.run();
    int score = game_controller.destroyScreen();
    std::cout << "Score: " << score;
    if (result == -2)
        std::cout << ", Quit\n\nOb-La-Di, Ob-La-Da, Life goes on brah" << std::endl;
    else if (result == -1)
        std::cout << ", Lost\n\nJUST DO IT. DONT LET YOUR DREAMS BE DREAMS.\nYesterday you said tommorrow, SO JUST DO IT" << std::endl;
    else if (result == 1)
        std::cout << ", Won\n\nGreat, kid! Don't get cocky.\nAlso don't eat my Strawberry Smiggles" << std::endl;

    return 0;
}
