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

private slots:
    void incomingConnection();

private:
    uint mCurrentPlayer;
    QTcpServer *mServer;

    void run();

};

#endif // GAMESERVER_H
