#ifndef SHOOT_H
#define SHOOT_H

#include <QPainter>
#include "entity.h"

class Tank;
class Explosion;
class Shoot : public Entity
{
    Q_OBJECT
public:
    Shoot(Tank *t, int strength = 70, int radius = 100);

    void fire(QPointF pos,int angel, int force);
    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual QRect boundingBox() const;
    virtual void  calculate(float speed, float windForce);

    inline Tank* tank() const { return mCreator; }

signals:
    void groundHit(QPointF p);
    void tankHit(QPointF p);
    void deathAt(QPointF p);
    void shoot(Shoot *s);

protected:
    virtual void  handleGroundCollision();
    virtual void  handleEntityCollision(Entity *e);
    virtual void  deathTrigger(bool withExplosion);

    inline int strength() const { return mStrength; }
    inline int radius() const   { return mRadius;   }

private:    
    int    mX;
    int    mY;
    float  mVx;
    float  mVy;
    Tank  *mCreator;
    int    mStrength;
    int    mRadius;
};

#endif // SHOOT_H
