#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "game.h"

#include <QTcpServer>
#include <QTcpSocket>

class GameServerClient;

class GameServer : public QObject, public Game
{
    Q_OBJECT
public:
    GameServer(uint n, uint m, uint k, unsigned short port = 55555);

    void run();

    void clientTurn(GameServerClient *client, QString msg);

private slots:
    void incomingConnection();

private:
    const uint mN, mM, mK;
    uint mCurrentPlayer;
    QTcpServer *mServer;

    std::vector<GameServerClient *> mClients;

    void sendTo(GameServerClient *c, QString msg);
    void sendToAll(QString msg);
};

#endif // GAMESERVER_H
