#ifndef GAMELOCAL_H
#define GAMELOCAL_H

#include "game.h"

class GameLocal : public Game
{
public:
    GameLocal(uint n, uint m, uint k);
    void run();

private:
    const uint mN, mM, mK;
};

#endif // GAMELOCAL_H
