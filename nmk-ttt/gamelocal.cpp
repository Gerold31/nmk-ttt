#include "gamelocal.h"

#include "nmk.h"

#include <iostream>

#include "QCoreApplication"

GameLocal::GameLocal(uint n, uint m, uint k):
    Game(new nmk(n, m, k)), mN(n), mM(m), mK(k)
{
}

void GameLocal::run()
{
    uint winner = 0;
    uint nextPlayer = 0;
    uint c[mN];
    do
    {
        nextPlayer++;
        do
        {
            std::cout << "Player " << nextPlayer << "'s turn" << std::endl;

            for(size_t i=0; i<mN; i++)
            {
                std::cin >> c[i];
            }
        }while(!mNmk->move(c, nextPlayer));

        if(nextPlayer >= mK)
            nextPlayer = 0;
    }while((winner = mNmk->checkWin(c)) == 0);

    if(winner == (uint)-1)
        std::cout << "draw" << std::endl;
    else
        std::cout << "Player " << winner << " won" << std::endl;

    exit(0);
}
