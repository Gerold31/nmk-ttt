#include "gameserver.h"

#include <QCoreApplication>

#include <iostream>

#include "nmk.h"
#include "gameserverclient.h"

GameServer::GameServer(uint n, uint m, uint k, QString name, unsigned short port):
    Game(new nmk(n, m, k, name))
{
    mServer = new QTcpServer();
    mServer->listen(QHostAddress::Any, port);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    mCurrentPlayer = 0;
}

void GameServer::run()
{
}

void GameServer::incomingConnection()
{
    QTcpSocket *socket = mServer->nextPendingConnection();

    socket->write(QString("%1\n").arg(mNmk->getN()).toUtf8());
    socket->flush();

    new GameServerClient(socket, this);
}

void GameServer::processMsg(QString msg, QTcpSocket *socket)
{
    msg = msg.replace('\n', "");

    nmk::ERROR error;

    switch(msg.at(0).toLatin1())
    {
    case 'n': // register new player
    {
        QString name = msg.section(' ', 1 , 1);
        uint id, session;
        if((error = mNmk->addPlayer(name, id, session))==nmk::ERROR::NONE)
            socket->write(QString("n %1 %2\n").arg(id).arg(session).toUtf8());

        break;
    }
    case 's': // request game state
    {
        socket->write(QString("s\n").toUtf8());
        error = mNmk->getState(socket);
        socket->write(QString("sx\n").toUtf8());
        break;
    }
    case 't': // turn
    {
        uint session = msg.section(' ', 1, 1).toUInt();
        uint t[mNmk->getN()];
        for(uint i=0; i<mNmk->getN(); i++)
        {
            t[i] = msg.section(' ', i+2, i+2).toUInt();
        }
        if((error = mNmk->turn(t, session))==nmk::ERROR::NONE)
            socket->write("t\n");
        break;
    }

    }

    if(error != nmk::ERROR::NONE)
    {
        socket->write(QString("e %1\n").arg((int)error).toUtf8());
    }
    socket->flush();
}
