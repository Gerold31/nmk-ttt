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

int nmk::checkWin()
{
    std::vector<uint> playerBuckets[mK];
    for(size_t i=0; i<pow(mM, mN); i++)
    {
        if(mMap[i] != 0)
        {
            playerBuckets[mMap[i]-1].push_back(i);
        }
    }
    uint *c_a = new uint[mM];
    uint *c_b = new uint[mM];
    uint *d = new uint[mM];

    uint winner = 0;

    for(size_t i=0; i<mK; i++)
    {
        if(playerBuckets[i].size() < mM)
            continue;

        for(auto a=playerBuckets[i].begin(); a!=playerBuckets[i].end() && winner == 0; ++a)
        {
            for(auto b=a+1; b!=playerBuckets[i].end() && winner == 0; ++b)
            {
                uint l = 2;
                coordToVec(c_a, *a);
                coordToVec(c_b, *b);

                if(sub(d, c_b, c_a) != 1)
                    continue;

                for(add(c_b, c_b, d); isValid(c_b); add(c_b, c_b, d))
                {
                    // find n
                    if(std::find(b+1, playerBuckets[i].end(), vecToCoord(c_b)) != playerBuckets[i].end())
                        l++;
                    else
                        break;
                }
                for(sub(c_a, c_a, d); isValid(c_a); sub(c_a, c_a, d))
                {
                    // find n
                    if(std::find(b+1, playerBuckets[i].end(), vecToCoord(c_a)) != playerBuckets[i].end())
                        l++;
                    else
                        break;
                }
                if(l > mM)
                    std::cout << "l > mM?!?" << std::cout;
                if(l == mM)
                    winner = i+1;
            }
        }
    }
    delete[] c_a;
    delete[] c_b;
    delete[] d;
    return winner == 0 ? mMoves == pow(mM, mN) ? -1 : 0 : winner;
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
