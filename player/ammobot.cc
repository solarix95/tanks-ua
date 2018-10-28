#include "ammobot.h"
#include <math.h>
#include <QDebug>

// --------------------------------------------------------------------------------
AmmoBot::AmmoBot(const QString &name)
    : Bot(name)
{
    setBotState(ABInit);
}

// --------------------------------------------------------------------------------
void AmmoBot::setTank(Tank *t)
{
    Bot::setTank(t);
    t->setColor(Qt::red);
    connect(t, SIGNAL(bulletsAdded()), this, SLOT(gotBullets()));
    setBotState(ABInit);
}

// --------------------------------------------------------------------------------
void AmmoBot::handleNewEnemy()
{
    if (mBotState != ABHuntTarget)
        return;

    Bot::handleNewEnemy();

    if ((direction() == DirUnknown) && (ammoBoxCount() > 0)) {
        setBotState(ABInit);
    }
}

// --------------------------------------------------------------------------------
void AmmoBot::handleStateChange()
{
    Player::handleStateChange();

    if (state() != StActive || !tank())
        return;

    switch (mBotState) {
    case ABInit: {
        if (ammoBoxCount() > 0) {
            selectNewAmmoBox(true);
            tank()->aimBot(targetPos());
            resetHits();
            setBotState(ABHuntAmmo);
        } else {
            setBotState(ABHuntTarget);
            huntTank();
        }
    } break;
    case ABHuntAmmo: {
        huntAmmo();
    } break;
    case ABHuntTarget: {
        huntTank();
    } break;
    }
    shoot();
}

// --------------------------------------------------------------------------------
void AmmoBot::handleAmmoBoxes()
{
    if (mBotState == ABHuntAmmo) {
        setBotState(ABInit);
        resetTarget();
    }
}

// --------------------------------------------------------------------------------
void AmmoBot::gotBullets()
{
    setBotState(ABHuntTarget);
    resetTarget();
}

// --------------------------------------------------------------------------------
void AmmoBot::huntTank()
{
    Q_ASSERT(enemyCount() > 0);
    if (selectNewTarget()) {
        tank()->aimBot(targetPos());
        tank()->setForce(tank()->force() + qrand() % 20 - 10);
        /*
        int randAngle = qrand() % 6 - 3;
        tank()->setAngle((direction() == DirRight ? 70 : 110) + randAngle);
        tank()->setForce(tank()->force() + qrand() % 20 - 15);
        */
        resetHits();
    } else if (lastTankHit().x() < 0) {
        float dist = (targetPos().x() - lastGroundHit().x())/50.0;
        int newForce = tank()->force() + (direction() == DirRight ? dist:-dist);
        if (newForce == 0)
            newForce = direction() == DirRight ? 3 : -3;
        tank()->setForce(newForce);
    }
}

// --------------------------------------------------------------------------------
void AmmoBot::huntAmmo()
{
    Q_ASSERT(ammoBoxCount() > 0);
    if (selectNewAmmoBox()) {
        int randAngle = qrand() % 6 - 3;
        tank()->setAngle((direction() == DirRight ? 70 : 110) + randAngle);
        tank()->setForce(tank()->force() + qrand() % 50 - 15);
        resetHits();
    } else if (lastHit().x() != -1) {
        float dist = (targetPos().x() - lastHit().x())/60.0;
        if (fabs(dist) < 2)
            dist = 2 * dist/fabs(dist);
        int newForce = tank()->force() + (direction() == DirRight ? dist:-dist);
        if (newForce == 0)
            newForce = direction() == DirRight ? 3 : -3;
        tank()->setForce(newForce);
    }
}

// --------------------------------------------------------------------------------
void AmmoBot::setBotState(AmmoBot::AmmoBotState st)
{
    mBotState= st;
}
