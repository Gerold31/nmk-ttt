#ifndef GAME_H
#define GAME_H

class nmk;
typedef unsigned int uint;

class Game
{
public:
    Game(nmk *nmk);
    virtual ~Game();

    virtual void run() = 0;

protected:
    nmk *mNmk;
};

#endif // GAME_H
