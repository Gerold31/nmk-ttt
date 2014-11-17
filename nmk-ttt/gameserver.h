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
    GameServer(uint n, uint m, uint k, QString name, unsigned short port);

    void processMsg(QString msg, QTcpSocket *socket);
    void removeClient(GameServerClient *client);

private slots:
    void incomingConnection();

private:
    uint mCurrentPlayer;
    QTcpServer *mServer;
    std::vector<GameServerClient *> mClients;

    void run();
    void informAllClients();

};

#endif // GAMESERVER_H
