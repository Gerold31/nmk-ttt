#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "game.h"

#include <QTcpSocket>

class GameClient : public Game
{
public:
    GameClient(QString ip, unsigned short port = 55555);

    void run();

private:
    QTcpSocket *mSocket;
};

#endif // GAMECLIENT_H
