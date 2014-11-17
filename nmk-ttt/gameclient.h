#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "game.h"

#include <QTcpSocket>
#include <QThread>

class GameClient : public QThread,  public Game
{
    Q_OBJECT
public:
    GameClient(QString ip, unsigned short port, QString sessionFile);

private slots:
    void processMsg();

private:
    uint mN;
    QTcpSocket *mSocket;
    QString mSession, mSessionFileName;
    bool mMultilineAnswer;

    void run();
};

#endif // GAMECLIENT_H
