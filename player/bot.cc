#include "bot.h"
#include "shoot.h"
#include <QDebug>

int Bot::mBotCount = 0;

// --------------------------------------------------------------------------------
Bot::Bot(const QString &name)
    : Player(name + QString(" #%1").arg(++mBotCount))
{
    mEnemyDir = DirUnknown;
    resetHits();
}

// --------------------------------------------------------------------------------
void Bot::handleNewEnemy()
{
    bool keepSameEnemy = false;
    for (int i=0; i<enemyCount(); i++) {
        if (enemyPos(i).x() == mTargetPos.x()) {
            keepSameEnemy = true;
            break;
        }
    }
    if (!keepSameEnemy)
        mEnemyDir = DirUnknown; // Problem: auch AmmoBox-Targeting betroffen. Muss vom konkreten Bot behandelt werden.
}

// --------------------------------------------------------------------------------
void Bot::groundHit(QPointF p)
{
    mLastGroundHit = p;
    mLastTankHit   = QPointF(-1,-1);
}

// --------------------------------------------------------------------------------
void Bot::tankHit(QPointF p)
{
    mLastTankHit = p;
}

// --------------------------------------------------------------------------------
void Bot::commonHit(QPointF p)
{
    mLastHit = p;
}

// --------------------------------------------------------------------------------
void Bot::shoot()
{
    Shoot *s = tank()->fire();
    connect(s, SIGNAL(groundHit(QPointF)), this, SLOT(groundHit(QPointF)));
    connect(s, SIGNAL(tankHit(QPointF)), this, SLOT(tankHit(QPointF)));
    connect(s, SIGNAL(deathAt(QPointF)), this, SLOT(commonHit(QPointF)));
}

// --------------------------------------------------------------------------------
void Bot::resetHits()
{
    mLastGroundHit = mLastTankHit = mLastHit = QPointF(-1,-1);
}

// --------------------------------------------------------------------------------
void Bot::resetTarget()
{
    mEnemyDir = DirUnknown;
    mTargetPos = QPointF(-1,-1);
}

// --------------------------------------------------------------------------------
bool Bot::selectNewTarget()
{
    Q_ASSERT(enemyCount() > 0);
    if (mEnemyDir == DirUnknown) {
        // setTargetPos(enemyPos(qrand() % enemyCount()));
        QPointF bestPos = enemyPos(0);
        int bestRate    = rateEnemyPos(bestPos);
        for (int i=1; i<enemyCount(); i++) {
            if (rateEnemyPos(enemyPos(i)) < bestRate) {
                bestPos  =  enemyPos(i);
                bestRate =  rateEnemyPos(bestPos);
            }
        }
        setTargetPos(bestPos);

        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
bool Bot::selectNewAmmoBox(bool force)
{
    Q_ASSERT(ammoBoxCount() > 0);
    if ((force || (mEnemyDir == DirUnknown)) && (ammoBoxCount() > 0)) {
        QPointF bestPos = ammoBoxPos(0);
        int bestRate = rateEnemyPos(bestPos);
        for (int i=1; i<ammoBoxCount(); i++) {
            if (rateEnemyPos(ammoBoxPos(i)) < bestRate) {
                bestPos  =  ammoBoxPos(i);
                bestRate =  rateEnemyPos(bestPos);
            }
        }
        setTargetPos(bestPos);
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
void Bot::setTargetPos(QPointF pos)
{
    mTargetPos = pos;
    mEnemyDir  = mTargetPos.x() > tank()->pos().x() ? DirRight : DirLeft;
}

// --------------------------------------------------------------------------------
int Bot::rateEnemyPos(const QPointF &ePos) const
/*
  Bewertung einer Ziel-Koordinate. Ein gutes Ziel
    - befindet sich auf der gleichen Höhe (Zielen funktioniert besser)
    - ist in der Nähe, jedoch nicht zu nah (Gefahr der Selbstzerstörung)

  Je kleiner der Returnwert, desto besser ist das Ziel
*/
{
    Q_ASSERT(tank());
    int yRate = abs(tank()->pos().y() - ePos.y());
    int xRate = abs(200 - abs(tank()->pos().x() - ePos.x()));
    return yRate + xRate;
}
