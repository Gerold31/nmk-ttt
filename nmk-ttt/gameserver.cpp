#include "gameserver.h"

#include <QCoreApplication>

#include <iostream>

#include "nmk.h"
#include "gameserverclient.h"

GameServer::GameServer(uint n, uint m, uint k, unsigned short port):
    Game(new nmk(n, m, k)), mN(n), mM(m), mK(k)
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
    if(mClients.size() == mK)
    {
        socket->disconnectFromHost();
    }else
    {
        GameServerClient *c = new GameServerClient(socket, this);
        mClients.push_back(c);
        sendTo(c, QString("p %1").arg(mClients.size()));
        if(mClients.size() == mK)
        {
            sendTo(mClients.at(0), QString("r %1").arg(mN));
            mClients.at(0)->setState(GameServerClient::STATE::TURN);
        }
        else
            sendToAll(QString("c %1").arg(mK - mClients.size()));
    }

}

void GameServer::clientTurn(GameServerClient *client, QString msg)
{
    if(mClients[mCurrentPlayer] != client)
        return;

    uint c[mN];
    msg = msg.replace('\n', "");
    for(size_t i=0; i<mN; i++)
    {
        c[i] = msg.section(' ', i, i).toUInt();
    }

    if(mNmk->move(c, mCurrentPlayer+1))
    {
        sendToAll(QString("m %1 %2").arg(mCurrentPlayer+1).arg(msg));
        mCurrentPlayer++;
        if(mCurrentPlayer >= mK)
            mCurrentPlayer = 0;

        uint winner = mNmk->checkWin();
        switch(winner)
        {
        case 0:
            sendTo(mClients[mCurrentPlayer], QString("r %1").arg(mN));
            mClients[mCurrentPlayer]->setState(GameServerClient::STATE::TURN);
            break;
        case (uint)-1:
            sendToAll("d");
        default:
            sendToAll(QString("w %1").arg(winner));
            for(auto i = mClients.begin(); i!= mClients.end(); ++i)
            {
                (*i)->getSocket()->waitForBytesWritten();
                (*i)->getSocket()->disconnectFromHost();
                QCoreApplication::quit();
            }
        }

    }else
    {
        sendTo(client, "i");
        sendTo(client, QString("r %1").arg(mN));
        client->setState(GameServerClient::STATE::TURN);
    }
}

void GameServer::sendTo(GameServerClient *c, QString msg)
{
    c->getSocket()->write((msg + "\n").toUtf8());
}

void GameServer::sendToAll(QString msg)
{
    for(auto i = mClients.begin(); i!= mClients.end(); ++i)
    {
        (*i)->getSocket()->write((msg + "\n").toUtf8());
    }
}
