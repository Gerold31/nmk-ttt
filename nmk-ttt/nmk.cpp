#include "nmk.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

#include <QTextStream>

uint pow(uint b, uint e)
{
    uint r = 1;
    while(e > 0)
    {
        r *= b;
        e--;
    }
    return r;
}

nmk::nmk(uint n, uint m, uint k, QString name) :
    mN(n), mM(m), mK(k), mName(name)
{
    mMap = new uint[pow(m, n)];
    mFile.setFileName(name + ".nmk");
    mFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
    if(mFile.isOpen())
    {
        QTextStream out(&mFile);
        out << mN << "\n";
        out << mM << "\n";
        out << mK << "\n";
        mFile.flush();
        mFile.close();
    }
    mCurrentPlayer = mMoves = 0;
}

nmk::ERROR nmk::addPlayer(QString name, uint &id, uint &session)
{
    if(mPlayers.size() == mK)
        return ERROR::SERVER_FULL;
    for(auto i=mPlayers.begin(); i!= mPlayers.end(); i++)
    {
        if((*i).second->getName() == name)
            return ERROR::NAME_TAKEN;
    }

    mFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
    if(!mFile.isOpen())
        return ERROR::INTERNAL_ERROR;

    QFile f(mName + "_" + name + ".session");
    f.open(QIODevice::Truncate | QIODevice::WriteOnly);
    if(!f.isOpen())
        return ERROR::INTERNAL_ERROR;

    do
    {
        session = rand()%8 | rand()%8 << 8 | rand()%8 << 16 | rand()%8 << 24;
    }while(mPlayers.count(session) != 0);

    id = mPlayers.size() + 1;
    mPlayers[session] = new Player(name, id, session);

    f.write(QString().setNum(session).toUtf8());
    f.close();


    QTextStream out(&mFile);
    out << id << " " << name << "\n";

    mFile.flush();
    mFile.close();

    return ERROR::NONE;
}

nmk::ERROR nmk::turn(uint *t, uint session)
{
    if(mPlayers.count(session) == 0)
        return ERROR::INVALID_SESSION;

    uint player = mPlayers[session]->getID();

    if(mCurrentPlayer != player-1)
        return ERROR::NOT_YOUR_TURN;
    if(mPlayers.size() != mK)
        return ERROR::GAME_NOT_RUNNING;

    uint coord = vecToCoord(t);

    if(mMap[coord] != 0)
        return ERROR::PLACE_TAKEN;
    if(!isValid(t))
        return ERROR::VECTOR_OUT_OF_BOUNDS;

    mFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
    if(!mFile.isOpen())
        return ERROR::INTERNAL_ERROR;

    mMap[coord] = player;
    mCurrentPlayer++;
    if(mCurrentPlayer == mK)
        mCurrentPlayer = 0;
    mMoves++;

    QTextStream out(&mFile);
    out << player;
    for(uint i=0; i<mN; i++)
        out << " " << t[i];
    out << "\n";
    mFile.flush();
    mFile.close();

    return ERROR::NONE;
}

int nmk::checkWin(uint *c)
{
    uint lastMove = vecToCoord(c);
    uint d[mN];
    uint a[mN];
    uint player = mMap[lastMove];
    for(uint i=0; i<pow(3, mN); i++)
    {
        bool first = true;
        uint k = i;
        for(uint j=0; j<mN; j++)
        {
            d[j] = k%3-1;
            k /= 3;
            if(first)
            {
                if(d[j] == -1)
                    break;
                if(d[j] == 1)
                    first = false;
            }
        }
        if(first)
            continue;

        bool stop = false;
        uint len = 1;

        coordToVec(a, lastMove);
        while(1)
        {
            add(a, a, d);
            if(!isValid(a))
                break;
            if(mMap[vecToCoord(a)] != player)
            {
                stop = true;
                break;
            }
            len++;
        }
        if(stop)
            continue;

        coordToVec(a, lastMove);
        while(1)
        {
            sub(a, a, d);
            if(!isValid(a))
                break;
            if(mMap[vecToCoord(a)] != player)
            {
                stop = true;
                break;
            }
            len++;
        }
        if(len == mM)
            return player;

    }
    return mMoves == pow(mM, mN) ? -1 : 0;
}

nmk::ERROR nmk::getState(QIODevice *stream)
{
    mFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!mFile.isOpen())
        return ERROR::INTERNAL_ERROR;

    stream->write(mFile.readAll());

    mFile.close();
    return ERROR::NONE;
}

void nmk::coordToVec(uint *v, uint c)
{
    for(size_t i=0; i<mN; i++)
    {
        v[i] = c%mM;
        c /= mM;
    }
}

uint nmk::vecToCoord(uint *v)
{
    uint coord = 0;
    for(size_t i=0; i<mN; i++)
        coord += v[i] * pow(mM, i);
    return coord;
}

bool nmk::isValid(uint *v)
{
    for(size_t i=0; i<mN; i++)
    {
        if(v[i] >= mM)
            return false;
    }
    return true;
}

uint nmk::sub(uint *l, uint *rl, uint *rr)
{
    /*
    std::cout << "(";
    for(size_t i=0; i<mN; i++)
        std::cout << rl[i] << ", ";
    std::cout << "\b\b) - (";

    for(size_t i=0; i<mN; i++)
        std::cout << rr[i] << ", ";
    std::cout << "\b\b) = (";
    */
    uint max = 0;
    for(size_t i=0; i<mN; i++)
    {
        l[i] = rl[i] - rr[i];
        uint d = rl[i] > rr[i] ? rl[i] - rr[i] : rr[i] - rl[i];
        max = d > max ? d : max;
        //std::cout << l[i] << ", ";
    }
    //std::cout << "\b\b), max = " << max << std::endl;
    return max;
}

uint nmk::add(uint *l, uint *rl, uint *rr)
{
    /*
    std::cout << "(";
    for(size_t i=0; i<mN; i++)
        std::cout << rl[i] << ", ";
    std::cout << "\b\b) + (";

    for(size_t i=0; i<mN; i++)
        std::cout << rr[i] << ", ";
    std::cout << "\b\b) = (";
    */
    uint max = 0;
    for(size_t i=0; i<mN; i++)
    {
        l[i] = rl[i] + rr[i];
        uint d = rl[i] > rr[i] ? rl[i] - rr[i] : rr[i] - rl[i];
        max = d > max ? d : max;
        //std::cout << l[i] << ", ";
    }
    //std::cout << "\b\b), max = " << max << std::endl;
    return max;
}
