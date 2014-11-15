#ifndef GAMESERVERCLIENT_H
#define GAMESERVERCLIENT_H

#include <QTcpSocket>

class GameServer;

class GameServerClient : public QObject
{
    Q_OBJECT
public:
    GameServerClient(QTcpSocket *socket, GameServer *server);

    enum class STATE
    {
        IDLE,
        TURN
    };

    void setState(STATE n){mState = n;}
    STATE getState(){return mState;}

    QTcpSocket *getSocket(){return mSocket;}

private slots:
    void readyRead();

private:
    QTcpSocket *mSocket;
    GameServer *mServer;
    STATE mState;
};
#endif // GAMESERVERCLIENT_H
