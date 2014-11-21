#include <QCoreApplication>


#include "gamelocal.h"
#include "gameclient.h"
#include "gameserver.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    srand(time(0));

    Game *g;

    while(1)
    {
        std::cout << "Enter Mode: \n\t0: Local\n\t1: Client\n\t2: Server" << std::endl;
        int mode;
        std::cin >> mode;
        uint n, m, k;
        if(mode == 0)
        {
            std::string name;
            std::cout << "Name: ";
            std::cin >> name;
            std::cout << "n (number of dimensions): ";
            std::cin >> n;
            std::cout << "m (size of dimensions): ";
            std::cin >> m;
            std::cout << "k (number of players): ";
            std::cin >> k;
            g = new GameLocal(n, m, k, name.c_str());
            g->run();
            break;
        }
        if(mode == 1)
        {
            std::string ip, sfile;
            unsigned short port;
            std::cout << "IP: ";
            std::cin >> ip;
            std::cout << "Port: ";
            std::cin >> port;
            std::cout << "Session file: ";
            std::cin >> sfile;
            g = new GameClient(ip.c_str(), port, sfile.c_str());
            ((GameClient *)g)->start();
            break;
        }
        if(mode == 2)
        {
            unsigned short port;
            std::string name;
            char c;
            std::cout << "Load game? [Y/n]: ";
            std::cin >> c;
            std::cout << "Name: ";
            std::cin >> name;
            if(c == 'n')
            {
                std::cout << "n (number of dimensions): ";
                std::cin >> n;
                std::cout << "m (size of dimensions): ";
                std::cin >> m;
                std::cout << "k (number of players): ";
                std::cin >> k;
                std::cout << "Port: ";
                std::cin >> port;
                g = new GameServer(n, m, k, name.c_str(), port);
            }else
            {
                std::cout << "Port: ";
                std::cin >> port;
                g = new GameServer(name.c_str(), port);
            }
            break;
        }
    }

    int ret = a.exec();
    delete g;
    return ret;
}
