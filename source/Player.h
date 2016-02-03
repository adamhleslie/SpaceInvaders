class Player
{
public:
    Player();
    virtual ~Player();
    int points;

private:
    virtual void parseInput(char input) = 0;
};