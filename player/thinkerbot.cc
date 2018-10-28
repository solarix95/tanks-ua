#include <QDebug>
#include "thinkerbot.h"
#include "shoot.h"

// --------------------------------------------------------------------------------
ThinkerBot::ThinkerBot(const QString &name)
    : Bot(name)
{
}

// --------------------------------------------------------------------------------
void ThinkerBot::setTank(Tank *t)
{
    Q_ASSERT(t);
    Bot::setTank(t);
    t->setColor(QColor("#00CC00"));
}

// --------------------------------------------------------------------------------
void ThinkerBot::handleStateChange()
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
        float dist = (targetPos().x() - lastGroundHit().x())/50.0;
        int newForce = tank()->force() + (direction() == DirRight ? dist:-dist);
        if (newForce == 0)
            newForce = direction() == DirRight ? 2 : -2;
        tank()->setForce(newForce);
    }
    shoot();
}
