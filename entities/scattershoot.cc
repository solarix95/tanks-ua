#include "scattershoot.h"

//-----------------------------------------------------------------------------
ScatterShoot::ScatterShoot(Tank *t, int strength, int radius)
    : Shoot(t,strength,radius)
{
}

//-----------------------------------------------------------------------------
void ScatterShoot::calculate(float speed, float windForce)
{
    float oldSpeedY = speedY();
    Shoot::calculate(speed,windForce);
    if (oldSpeedY > 0 && speedY() < 0)
        createChilds();
}

//-----------------------------------------------------------------------------
void ScatterShoot::createChilds()
{
    createChild(+1);
    createChild(+2);
    createChild(-1);
    createChild(-2);
}

//-----------------------------------------------------------------------------
void ScatterShoot::createChild(int index)
{
    Shoot *rightShoot = new Shoot(tank(),strength(),radius());
    QPointF newPos = pos();
    newPos.setX(newPos.x() + (index*2*boundingBox().width()) + (index*5));
    rightShoot->setPos(newPos);
    rightShoot->setSpeedX(speedX()+(index*3));
    rightShoot->setSpeedY(0);
    emit shoot(rightShoot);
}
