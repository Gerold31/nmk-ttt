#ifndef GAMESERVERCLIENT_H
#define GAMESERVERCLIENT_H

#include <QTcpSocket>

class GameServer;

class GameServerClient : public QObject
{
    Q_OBJECT
public:
    GameServerClient(QTcpSocket *socket, GameServer *server);
    ~GameServerClient();

    QTcpSocket *getSocket(){return mSocket;}

private slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *mSocket;
    GameServer *mServer;
};
#endif // GAMESERVERCLIENT_H
