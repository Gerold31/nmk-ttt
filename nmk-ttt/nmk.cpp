#include "nmk.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

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

nmk::nmk(uint n, uint m, uint k) :
    mN(n), mM(m), mK(k)
{
    mMap = new uint[pow(m, n)];
}

bool nmk::move(uint *c, uint k)
{
    int coord = vecToCoord(c);
    if(mMap[coord] != 0 || !isValid(c))
        return false;
    mMap[coord] = k;
    mMoves++;
    return true;
}

bool nmk::move(uint c, uint k)
{
    uint v[mK];
    coordToVec(v, c);
    return move(v, k);
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
