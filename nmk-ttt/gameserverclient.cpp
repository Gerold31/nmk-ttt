#include "gameserverclient.h"

#include "gameserver.h"

#include <iostream>

GameServerClient::GameServerClient(QTcpSocket *socket, GameServer *server)
{
    mSocket = socket;
    mServer = server;
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void GameServerClient::readyRead()
{
    while(mSocket->canReadLine())
    {
        mServer->processMsg(mSocket->readLine(), mSocket);
    }
}
