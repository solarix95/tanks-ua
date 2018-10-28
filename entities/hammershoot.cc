#include "hammershoot.h"

HammerShoot::HammerShoot(Tank *t, int level, int strength, int radius)
    : Shoot(t,strength,radius), mLevel(level)
{
}

//-----------------------------------------------------------------------------
void HammerShoot::handleGroundCollision()
{
    Shoot::handleGroundCollision();
    createChild();
}

//-----------------------------------------------------------------------------
void HammerShoot::handleEntityCollision(Entity *e)
{
    Shoot::handleEntityCollision(e);
    if (e->isSolid())
        createChild();
}

//-----------------------------------------------------------------------------
void HammerShoot::createChild()
{
    if (mLevel > 0) {
        Shoot *childShoot = new HammerShoot(tank(),mLevel - 1, strength(),radius());
        QPointF shootPos = pos();
        shootPos.setY(shootPos.y() + 10);
        childShoot->setPos(shootPos);
        childShoot->setSpeedX(speedX()/10);
        childShoot->setSpeedY(10*mLevel);
        emit shoot(childShoot);
        mLevel = 0; // create only one Child..
    }
}
