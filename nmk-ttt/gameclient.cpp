#include "gameclient.h"

#include <QCoreApplication>
#include <QHostAddress>

#include <iostream>

GameClient::GameClient(QString ip, unsigned short port) :
    Game(nullptr)
{
    mSocket = new QTcpSocket();
    mSocket->connectToHost(QHostAddress(ip), port);
}

void GameClient::run()
{
    if(!mSocket->waitForConnected())
    {
        std::cout << "Connection failed" << std::endl;
        exit(-1);
    }

    while(mSocket->isOpen())
    {
        mSocket->waitForReadyRead(-1);
        while(mSocket->canReadLine())
        {
            QString msg = mSocket->readLine();
            msg = msg.replace('\n', "");
            switch(msg.at(0).toLatin1())
            {
            case 'c':
                std::cout << "Wating for " << msg.section(' ', 1, 1).toStdString() << " more players to connect" << std::endl;
                break;
            case 'p':
                std::cout << "You are Player " << msg.section(' ', 1, 1).toStdString() << std::endl;
                break;
            case 'w':
                std::cout << "Player " << msg.section(' ', 1, 1).toStdString() << " won!" << std::endl;
                mSocket->disconnectFromHost();
                mSocket->close();
                break;
            case 'd':
                std::cout << "Draw!" << std::endl;
                mSocket->disconnectFromHost();
                mSocket->close();
                break;
            case 'r':
            {
                std::cout << "Your turn" << std::endl;
                uint n = msg.section(' ', 1, 1).toUInt();
                msg = "";
                for(uint i=0; i<n; i++)
                {
                    uint c;
                    std::cin >> c;
                    msg += QString().setNum(c) + " ";
                }
                msg += "\n";
                mSocket->write(msg.toUtf8());
                mSocket->waitForBytesWritten();
                break;
            }
            case 'i':
                std::cout << "Invalid Turn!" << std::endl;
                break;
            case 'm':
                std::cout << "Player " << msg.section(' ', 1, 1).toStdString() << "'s turn: " << msg.section(' ', 2).toStdString() << std::endl;
                break;
            default:
                std::cout << "Unknown command " << msg.toStdString() << std::endl;
            }
        }
    }
    exit(0);
}
