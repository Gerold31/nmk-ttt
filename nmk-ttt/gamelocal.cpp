#include "gamelocal.h"

#include "nmk.h"

#include <iostream>

#include "QCoreApplication"

GameLocal::GameLocal(uint n, uint m, uint k, QString name):
    Game(new nmk(n, m, k, name)), mN(n), mM(m), mK(k)
{
}

void GameLocal::run()
{
    uint winner = 0;
    uint nextPlayer = 0;
    uint c[mN];
    uint s[mK];
    for(uint i=0; i<mK; i++)
    {
        uint j;
        mNmk->addPlayer(QString().setNum(i), j, s[i]);
    }
    do
    {
        nmk::ERROR e = nmk::ERROR::NONE;
        do
        {
            if(e != nmk::ERROR::NONE)
            {
                std::cout << "Error " << (int)e << std::endl;
            }
            std::cout << "Player " << nextPlayer+1 << "'s turn" << std::endl;

            for(size_t i=0; i<mN; i++)
            {
                std::cin >> c[i];
            }
        }while((e=mNmk->turn(c, s[nextPlayer])) != nmk::ERROR::NONE);

        nextPlayer++;

        if(nextPlayer >= mK)
            nextPlayer = 0;
    }while((winner = mNmk->getWinner()) == 0);

    if(winner == (uint)-1)
        std::cout << "draw" << std::endl;
    else
        std::cout << "Player " << winner << " won" << std::endl;

    exit(0);
}
