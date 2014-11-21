#include "game.h"

#include "nmk.h"

Game::Game(nmk *nmk):
    mNmk(nmk)
{
}

Game::~Game()
{
    delete mNmk;
}
