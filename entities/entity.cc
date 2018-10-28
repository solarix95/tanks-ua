#include <QDebug>
#include "entity.h"
#include "explosion.h"
#include <math.h>

// --------------------------------------------------------------------------------
#define MAX_HEALTH   9999
#define NORM_WEIGHT     1

int Entity::sId = 1;

// --------------------------------------------------------------------------------
Entity::Entity(QObject *parent) :
    QObject(parent)
{
    mId          = sId++;
    mIsSolid     = true;
    mEatsBullets = false;
    mSpeed       = QPointF(0,0);
    mPos         = QPointF(-1,-1);
    mState       = StInit;
    mWeight      = NORM_WEIGHT;
    mHealth      = MAX_HEALTH;
    mStateTime.start();
}

// --------------------------------------------------------------------------------
void Entity::tdPaint(QPainter&, const QSize&)
{
}

// --------------------------------------------------------------------------------
void Entity::buPaint(QPainter&, const QRect &)
{
}

// --------------------------------------------------------------------------------
void Entity::calculate(float speed, float windForce)
{
    QPointF newPos;
    if (calcStep(mPos,newPos,mSpeed,speed,mWeight,windForce))  {           // if moving..
        Q_ASSERT(speed > 0);
        Q_ASSERT(newPos != mPos);

        // Test every Pixel between old and new position;
        int   steps = sqrt(pow((mPos.x()-newPos.x()),2) + pow((mPos.y()-newPos.y()),2)) + 1;
        Q_ASSERT(steps >= 0);
        float dx    = (newPos.x()-mPos.x())/steps;
        float dy    = (newPos.y()-mPos.y())/steps;
        bool collisionWithGround = false;
        for (int s=0; s<=steps; s++) {
            QPointF testPoint(mPos.x() + s*dx, mPos.y()+s*dy);
            if (testPoint.y() < 0) {
                setPos(testPoint);
                handleGroundCollision();
                return;
            }

            emit testCollision(testPoint,&collisionWithGround);
            if (collisionWithGround) {
                setPos(testPoint);
                handleGroundCollision();
                return;
            }

            if (isSolid()) {
                Entity *collisionWith = NULL;
                emit testCollision(this, &collisionWith);
                if (collisionWith) {
                    setPos(testPoint);
                    handleEntityCollision(collisionWith);
                    collisionWith->hitBy(this);
                    if (isSolid() && collisionWith->isSolid())
                        return;
                }
            }
        }

        Q_ASSERT(newPos.y() >= 0);
        setPos(newPos);
    }
}

// --------------------------------------------------------------------------------
bool Entity::handleExplosion(Explosion *e)
{
    if (this == e) // I cannot destroy myself..
        return false;

    float dist = sqrt(pow(pos().x() - e->pos().x(),2) + pow(pos().y() - e->pos().y(),2));

    if (dist > e->radius())
        return false;

    decHealth(e->strength() * (e->radius()-dist)/(float)e->radius(), e);

    return true;
}

// --------------------------------------------------------------------------------
void Entity::handleDeath()
{
    setState(StDead);
}

// --------------------------------------------------------------------------------
void Entity::decHealth(int d, Entity */*source*/)
{
    if (mHealth <= 0)
        return;

    mHealth -= d;
    if (mHealth <= 0) {
        mHealth = 0;
        handleDeath();
    }
}

// --------------------------------------------------------------------------------
bool Entity::calcStep(const QPointF &oldPos, QPointF &newPos, QPointF &speed, float gameSpeed, float weight, float windForce)
{
    if (speed.x()*speed.x()+speed.y()*speed.y()) {
        newPos  = QPointF(oldPos.x() + gameSpeed*speed.x(),oldPos.y() + gameSpeed*speed.y());
        speed.setX(speed.x()+gameSpeed*windForce/100/weight);  // wind
        speed.setY(speed.y()-gameSpeed*0.9*weight/1.0);        // gravity
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
void Entity::handleGroundCollision()
{
    setSpeedX(0);
    setSpeedY(0);
}

// --------------------------------------------------------------------------------
void Entity::handleEntityCollision(Entity *e)
{
}

// --------------------------------------------------------------------------------
void Entity::handleStateChange()
{
}

// --------------------------------------------------------------------------------
void Entity::hitBy(Entity *e)
{
}

// --------------------------------------------------------------------------------
void Entity::setState(int s)
{
    if (mState != StDead && mState != s) {
        mState = s;
        handleStateChange();
        mStateTime.start();
    }
}
