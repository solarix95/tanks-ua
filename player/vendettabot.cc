#include "vendettabot.h"
#include <QDebug>

// --------------------------------------------------------------------------------
VendettaBot::VendettaBot(const QString &name)
    : Bot(name)
{
}

// --------------------------------------------------------------------------------
void VendettaBot::setTank(Tank *t)
{
    Bot::setTank(t);
    t->setColor(QColor("#FFCC66"));
    connect(t, SIGNAL(tankHit(Tank*)), this, SLOT(hitBy(Tank*)));
}

// --------------------------------------------------------------------------------
void VendettaBot::hitBy(Tank *target)
{
    Q_ASSERT(target);
    Q_ASSERT(tank());
    if (targetPos() == target->pos())
        return;

    // qDebug() << "VENDETTTAAAA" << target->pos();
    setTargetPos(target->pos());
    tank()->aimBot(targetPos());
    groundHit(target->pos());
}

// --------------------------------------------------------------------------------
void VendettaBot::handleStateChange()
{
    Player::handleStateChange();

    if (state() != StActive || !tank())
        return;

    Q_ASSERT(enemyCount() > 0);
    if (selectNewTarget()) {
        int randAngle = qrand() % 6 - 3;
        tank()->setAngle((direction() == DirRight ? 70 : 110) + randAngle);
        tank()->setForce(tank()->force() + qrand() % 50 - 15);
        resetHits();
    } else if (lastTankHit().x() < 0) {
        float dist = (targetPos().x() - lastGroundHit().x())/40.0;
        int newForce = tank()->force() + (direction() == DirRight ? dist:-dist);
        if (newForce == 0)
            newForce = direction() == DirRight ? 2 : -2;
        tank()->setForce(newForce);
    }
    shoot();
}
