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
        GAME_FINISHED,
        CORRUPT_SAVE_GAME,
    };

    nmk(uint n, uint m, uint k, QString name);
    nmk(QString name);
    ~nmk();


    ERROR addPlayer(QString name, uint &id, uint &session);
    ERROR turn(uint *t, uint session);
    ERROR getState(QIODevice *stream);

    uint getWinner(){return mWinner;}

    uint getN(){return mN;}
    uint getM(){return mM;}
    uint getK(){return mK;}
    QString getName(){return mName;}

    uint getPlayerCount(){return mPlayers.size();}
    uint getCurrentPlayer(){return mCurrentPlayer+1;}

    static const char *errorToString(ERROR e);

private:
    uint *mMap;
    uint mN, mM, mK;
    const QString mName;
    uint mMoves;
    uint mCurrentPlayer;
    uint mWinner;

    QFile mFile;

    void coordToVec(uint *v, uint c);
    uint vecToCoord(uint *v);
    bool isValid(uint *v);

    int checkWin(uint *lastMove);

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
