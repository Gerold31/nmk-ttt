#ifndef GAMELOCAL_H
#define GAMELOCAL_H

#include "game.h"

#include <QString>

class GameLocal : public Game
{
public:
    GameLocal(uint n, uint m, uint k, QString name);

private:
    const uint mN, mM, mK;
    void run();
};

#endif // GAMELOCAL_H
