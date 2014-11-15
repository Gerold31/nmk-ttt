#include "gameserverclient.h"

#include "gameserver.h"

#include <iostream>

GameServerClient::GameServerClient(QTcpSocket *socket, GameServer *server)
{
    mSocket = socket;
    mServer = server;
    mState = STATE::IDLE;
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void GameServerClient::readyRead()
{
    /*
    if(mState == STATE::IDLE)
        mSocket->readAll();
    else if(mState == STATE::TURN)
    {
    */
        while(mSocket->canReadLine())
        {
            mServer->clientTurn(this, mSocket->readLine());
            mState = STATE::IDLE;
        }
    //}
}
