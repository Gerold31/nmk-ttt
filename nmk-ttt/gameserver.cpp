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
    QObject::connect(mServer, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

GameServer::GameServer(QString name, unsigned short port):
    Game(new nmk(name))
{
    mServer = new QTcpServer();
    mServer->listen(QHostAddress::Any, port);
    QObject::connect(mServer, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

GameServer::~GameServer()
{
    for(auto i = mClients.begin(); i!= mClients.end(); i++)
    {
        delete *i;
    }
    mServer->close();
    delete mServer;
}

void GameServer::run()
{
}

void GameServer::incomingConnection()
{
    QTcpSocket *socket = mServer->nextPendingConnection();

    socket->write(QString("%1\n").arg(mNmk->getN()).toUtf8());
    socket->flush();
    socket->waitForBytesWritten();

    mClients.push_back(new GameServerClient(socket, this));
}

void GameServer::removeClient(GameServerClient *client)
{
    for(auto i=mClients.begin(); i!= mClients.end(); i++)
    {
        if(*i == client)
        {
            mClients.erase(i);
            break;
        }
    }
}

void GameServer::processMsg(QString msg, QTcpSocket *socket)
{
    msg = msg.replace('\n', "");

    nmk::ERROR error = nmk::ERROR::NONE;

    switch(msg.at(0).toLatin1())
    {
    case 'n': // register new player
    {
        QString name = msg.section(' ', 1 , 1);
        uint id, session;
        if((error = mNmk->addPlayer(name, id, session))==nmk::ERROR::NONE)
        {
            socket->write(QString("n %1 %2\n").arg(id).arg(session).toUtf8());
            informAllClients();
        }

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
        error = mNmk->turn(t, session);
        if(error == nmk::ERROR::NONE)
        {
            informAllClients();
        }
        break;
    }
    case 'w': // check winner
        socket->write(QString("w %1\n").arg(mNmk->getWinner()).toUtf8());
        break;
    case 'i':
        if(mNmk->getPlayerCount() < mNmk->getK())
        {
            socket->write(QString("p %1\n").arg(mNmk->getK() - mNmk->getPlayerCount()).toUtf8());
        }else if(mNmk->getWinner() != 0)
        {
            socket->write(QString("w %1\n").arg(mNmk->getWinner()).toUtf8());
        }else
        {
            socket->write(QString("t %1\n").arg(mNmk->getCurrentPlayer()).toUtf8());
        }
        break;
    }

    if(error != nmk::ERROR::NONE)
    {
        socket->write(QString("e %1\n").arg((int)error).toUtf8());
    }
    socket->flush();
    socket->waitForBytesWritten();
}

void GameServer::informAllClients()
{
    for(auto i=mClients.begin(); i!=mClients.end(); ++i)
    {
        processMsg("i", (*i)->getSocket());
    }
}
