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

    uint n = -1;
    while(n == -1)
    {
        if(!mSocket->waitForReadyRead())
        {
            std::cout << "Connection timed out" << std::endl;
            exit(-1);
        }
        if(mSocket->canReadLine())
        {
            QString line = mSocket->readLine();
            n = line.toUInt();
        }

    }

    char c = 'h';
    bool expectAnswer;
    while(1)
    {
        expectAnswer = true;
        switch(c)
        {
        case 'h':
            std::cout << "Possible commands:" << std::endl;
            std::cout << "\tn <Name>                 - register new player" << std::endl;
            std::cout << "\ts                        - get current game state" << std::endl;
            std::cout << "\tt <session> <x1> .. <xn> - make a turn" << std::endl;
            std::cout << "\tw                        - check if somebody won" << std::endl;
            std::cout << "\tq                        - quit" << std::endl;
            std::cout << "\th                        - show this" << std::endl;
            expectAnswer = false;
            break;
        case 'n':
        {
            std::string name;
            std::cin >> name;
            mSocket->write((QString("n ") + name.c_str() + "\n").toUtf8());
            break;
        }
        case 's':
            mSocket->write(QString("s\n").toUtf8());
            break;
        case 't':
        {
            QString msg = "t";
            uint t;
            for(uint i=0; i<n+1; i++)
            {
                std::cin >> t;
                msg += " " + QString().setNum(t);
            }
            mSocket->write((msg + "\n").toUtf8());
            break;
        }
        case 'q':
            exit(0);
        }
        mSocket->flush();
        bool multilineAnswer = false;
        while(expectAnswer)
        {
            if(!mSocket->waitForReadyRead())
            {
                std::cout << "Connection timed out" << std::endl;
                exit(-1);
            }
            while(mSocket->canReadLine())
            {
                QString msg = mSocket->readLine();

                if(multilineAnswer)
                {
                    if(msg.startsWith("sx"))
                        multilineAnswer = false;
                    else
                        std::cout << msg.toStdString();
                }else
                {
                    switch(msg.at(0).toLatin1())
                    {
                    case 'n':
                        std::cout << "You are Player " << msg.section(' ', 1, 1).toStdString() << " with session " << msg.section(' ', 2, 2).toStdString();
                        break;
                    case 's':
                        multilineAnswer = true;
                        break;
                    case 'e':
                        std::cout << "Error: " << msg.section(' ', 1, 1).toStdString();
                        break;
                    case 'w':
                        if(msg.section(' ', 1, 1).toUInt() == 0)
                            std::cout << "No Winner" << std::endl;
                        else if(msg.section(' ', 1, 1).toUInt() == (uint)-1)
                            std::cout << "Draw!" << std::endl;
                        else
                            std::cout << "Player " << msg.section(' ', 1, 1).toUInt() << " won!" << std::endl;

                        break;
                    default:
                        std::cout << "Unknown reply " << msg.toStdString();
                        break;
                    }
                }
                if(!multilineAnswer)
                    expectAnswer = false;
            }
        }
        std::cin >> c;
    }
    exit(0);
}
