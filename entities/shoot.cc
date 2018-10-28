#include <QDebug>
#include "shoot.h"
#include "explosion.h"
#include "camera.h"
#include <math.h>

#define SIZE 10

// --------------------------------------------------------------------------------
Shoot::Shoot(Tank *t, int strength, int radius)
{
    mX       = -1;
    mY       = -1;
    mCreator = t;
    mStrength= strength;
    mRadius  = radius;
    setWeight(3.0);
}

// --------------------------------------------------------------------------------
void Shoot::fire(QPointF pos, int angel, int force)
{
    setPos(pos);

    if (!force) {
        deathTrigger(true);
        return;
    }

    float vx = cos(angel/180.0*(3.1415)) * force;
    float vy = sin(angel/180.0*(3.1415)) * force;

    setSpeedX(vx);
    setSpeedY(vy);
}

// --------------------------------------------------------------------------------
void Shoot::buPaint(QPainter &p, const QRect &region)
{
    if (!region.contains(pos().toPoint()))
        return;

    p.save();
    p.translate(pos().x()-SIZE/2,pos().y()-SIZE/2);
    p.setPen(QPen());
    p.setBrush(Qt::black);
    p.drawEllipse(0,0,SIZE,SIZE);
    p.restore();
}

// --------------------------------------------------------------------------------
void Shoot::handleGroundCollision()
{
    emit groundHit(pos());
    deathTrigger(true);
}

// --------------------------------------------------------------------------------
void Shoot::handleEntityCollision(Entity *e)
{
    if (!e->isSolid())
        return;

    if (e->eatsBullets()) {
        deathTrigger(false);
        return;
    }

    emit tankHit(e->pos());
    deathTrigger(true);
}

// --------------------------------------------------------------------------------
void Shoot::deathTrigger(bool withExplosion)
{
    if (withExplosion)
        emit explosion(new Explosion(mCreator,pos(),mStrength,mRadius));
    emit deathAt(pos());
    deleteLater();
}

// --------------------------------------------------------------------------------
QRect Shoot::boundingBox() const
{
    return QRect(pos().x()-SIZE/2,pos().y()-SIZE/2,SIZE,SIZE);
}

// --------------------------------------------------------------------------------
void Shoot::calculate(float speed, float windForce)
{
    Entity::calculate(speed, windForce);
    emit setCamera(pos().x(),Camera::Soft);
}

