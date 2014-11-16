#ifndef NMK_H
#define NMK_H

#include <QFile>

typedef unsigned int uint;

class nmk
{
public:
    enum class ERROR
    {
        NONE = 0,
        SERVER_FULL,
        NAME_TAKEN,
        INTERNAL_ERROR,
        NOT_YOUR_TURN,
        GAME_NOT_RUNNING,
        VECTOR_OUT_OF_BOUNDS,
        PLACE_TAKEN,
        INVALID_SESSION,
    };

    nmk(uint n, uint m, uint k, QString name);
    nmk(QString name);


    ERROR addPlayer(QString name, uint &id, uint &session);
    ERROR turn(uint *t, uint session);
    ERROR getState(QIODevice *stream);


    int checkWin(uint *lastMove);

    uint getN(){return mN;}
    uint getM(){return mM;}
    uint getK(){return mK;}
    QString getName(){return mName;}

private:
    uint *mMap;
    const uint mN, mM, mK;
    const QString mName;
    uint mMoves;
    uint mCurrentPlayer;

    QFile mFile;

    void coordToVec(uint *v, uint c);
    uint vecToCoord(uint *v);
    bool isValid(uint *v);

    // l = rl - rr
    uint sub(uint *l, uint *rl, uint *rr);
    // l = rl + rr
    uint add(uint *l, uint *rl, uint *rr);

    class Player
    {
    public:
        Player(const QString name, const uint id, const uint session) :
            mName(name), mID(id), mSession(session){}

        QString getName(){return mName;}
        uint getID(){return mID;}
        uint getSession(){return mSession;}

    private:
        const QString mName;
        const uint mID;
        const uint mSession;
    };

    std::map<uint, Player *> mPlayers;

};

#endif // NMK_H
