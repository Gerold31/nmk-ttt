#ifndef GAMESERVERCLIENT_H
#define GAMESERVERCLIENT_H

#include <QTcpSocket>

class GameServer;

class GameServerClient : public QObject
{
    Q_OBJECT
public:
    GameServerClient(QTcpSocket *socket, GameServer *server);

private slots:
    void readyRead();

private:
    QTcpSocket *mSocket;
    GameServer *mServer;
};
#endif // GAMESERVERCLIENT_H
