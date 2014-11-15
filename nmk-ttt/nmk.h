#ifndef NMK_H
#define NMK_H

typedef unsigned int uint;

class nmk
{
public:
    nmk(uint n, uint m, uint k);

    bool move(uint *c, uint k);
    bool move(uint c, uint k);

    int checkWin();

private:
    uint *mMap;
    const uint mN, mM, mK;
    uint mMoves;

    void coordToVec(uint *v, uint c);
    uint vecToCoord(uint *v);
    bool isValid(uint *v);

    // l = rl - rr
    uint sub(uint *l, uint *rl, uint *rr);
    // l = rl + rr
    uint add(uint *l, uint *rl, uint *rr);

};

#endif // NMK_H
