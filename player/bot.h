#ifndef BOT_H
#define BOT_H

#include "player.h"

class Bot : public Player
{
    Q_OBJECT
public:
    Bot(const QString &name);

    virtual void handleNewEnemy();

protected slots:
    virtual void groundHit(QPointF p);
    virtual void tankHit(QPointF p);
    virtual void commonHit(QPointF p);

protected:
    void shoot();
    void resetHits();
    void resetTarget();
    bool selectNewTarget();
    bool selectNewAmmoBox(bool force = false);
    void setTargetPos(QPointF pos);

    inline QPointF lastGroundHit() const { return mLastGroundHit; }
    inline QPointF lastTankHit() const   { return mLastTankHit;   }
    inline QPointF lastHit() const       { return mLastHit;       }
    inline QPointF targetPos() const     { return mTargetPos;     }

    typedef enum eEnemyDirection {
        DirUnknown,   // kein "Feind" (Neuer Feind muss gewählt werden)
        DirLeft,      // der Feind liegt links...
        DirRight      // ..
    } EnemyDirection;

    inline EnemyDirection direction() const { return mEnemyDir; }

private:
    int rateEnemyPos(const QPointF &pos) const;

    QPointF        mLastGroundHit;
    QPointF        mLastTankHit;
    QPointF        mLastHit;

    QPointF        mTargetPos;
    EnemyDirection mEnemyDir;

    static int     mBotCount;
};

#endif // BOT_H
