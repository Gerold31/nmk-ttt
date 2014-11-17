#include "gameclient.h"

#include <QCoreApplication>
#include <QHostAddress>
#include <QFile>

#include <iostream>

#include "nmk.h"

GameClient::GameClient(QString ip, unsigned short port, QString sessionFile) :
    Game(nullptr)
{
    mN = -1;
    mSocket = new QTcpSocket();
    mSocket->connect(mSocket, SIGNAL(readyRead()), this, SLOT(processMsg()));
    mSocket->connectToHost(QHostAddress(ip), port);

    QFile f(sessionFile);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    if(f.isOpen())
    {
        mSession = f.readLine().replace('\n', "");
        f.close();
    }else
        mSession = "";
    mSessionFileName = sessionFile;
    mMultilineAnswer = false;
}

void GameClient::run()
{
    if(!mSocket->waitForConnected())
    {
        std::cout << "Connection failed" << std::endl;
        exit(-1);
    }

    char c = 'h';
    while(1)
    {
        switch(c)
        {
        case 'h':
            std::cout << "Possible commands:" << std::endl;
            std::cout << "\tn <Name>       - register new player" << std::endl;
            std::cout << "\ti              - get current game state" << std::endl;
            std::cout << "\ts              - get full turn history" << std::endl;
            std::cout << "\tt <x1> .. <xn> - make a turn" << std::endl;
            std::cout << "\tw              - check if somebody won" << std::endl;
            std::cout << "\tq              - quit" << std::endl;
            std::cout << "\th              - show this" << std::endl;
            break;
        case 'n':
        {
            std::string name;
            std::cin >> name;
            mSocket->write((QString("n ") + name.c_str() + "\n").toUtf8());
            break;
        }
        case 'i':
            mSocket->write(QString("i\n").toUtf8());
            break;
        case 's':
            mSocket->write(QString("s\n").toUtf8());
            break;
        case 't':
        {
            QString msg = "t " + mSession;
            uint t;
            for(uint i=0; i<mN; i++)
            {
                std::cin >> t;
                msg += " " + QString().setNum(t);
            }
            mSocket->write((msg + "\n").toUtf8());
            break;
        }
        case 'w':
            mSocket->write(QString("w\n").toUtf8());
            break;
        case 'q':
            QCoreApplication::quit();
            break;
        }
        //mSocket->flush();
        //mSocket->waitForBytesWritten();
        std::cin >> c;
    }
    QCoreApplication::quit();
}

void GameClient::processMsg()
{
    while(mSocket->canReadLine())
    {
        QString msg = mSocket->readLine().replace('\n', "");

        if(mN == (uint)-1)
        {
            mN = msg.toUInt();
            continue;
        }

        if(mMultilineAnswer)
        {
            if(msg.startsWith("sx"))
            {
                std::cout << "==== END ====" << std::endl;
                mMultilineAnswer = false;
            }
            else
                std::cout << msg.toStdString() << std::endl;
        }else
        {
            switch(msg.at(0).toLatin1())
            {
            case 'n':
            {
                std::cout << "You are Player " << msg.section(' ', 1, 1).toStdString() << std::endl;
                mSession = msg.section(' ', 2, 2);

                QFile f(mSessionFileName);
                f.open(QIODevice::WriteOnly | QIODevice::Text);
                if(f.isOpen())
                {
                    f.write(msg.section(' ', 2, 2).toUtf8());
                    f.flush();
                    f.close();
                }else
                {
                    std::cout << "Failed to save session, session = " << mSession.toStdString() << std::endl;
                }
            }
                break;
            case 's':
                mMultilineAnswer = true;
                std::cout << "====START====" << std::endl;
                break;
            case 'e':
                std::cout << "Error: " << nmk::errorToString((nmk::ERROR)msg.section(' ', 1, 1).toInt()) << std::endl;
                break;
            case 'w':
                if(msg.section(' ', 1, 1).toUInt() == 0)
                    std::cout << "No Winner" << std::endl;
                else if(msg.section(' ', 1, 1).toUInt() == (uint)-1)
                    std::cout << "Draw!" << std::endl;
                else
                    std::cout << "Player " << msg.section(' ', 1, 1).toUInt() << " won!" << std::endl;

                break;
            case 'p':
                std::cout << "Waiting for " << msg.section(' ', 1, 1).toStdString() << " more Players" << std::endl;
                break;
            case 't':
                std::cout << "Player " << msg.section(' ', 1, 1).toStdString() << "'s turn" << std::endl;
                break;
            default:
                std::cout << "Unknown reply " << msg.toStdString() << std::endl;
                break;
            }
        }
    }
}
