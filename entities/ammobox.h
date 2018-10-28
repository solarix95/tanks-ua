#ifndef AMMOBOX_H
#define AMMOBOX_H

#include "entity.h"
#include "bullet.h"

class AmmoBox : public Entity
{
    Q_OBJECT
public:
    AmmoBox(QPointF pos, Bullet::GunType t);
    static QColor color();

    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual QRect boundingBox() const;
    virtual void calculate(float speed, float windForce);
    virtual void hitBy(Entity *e);

private:
     Bullet::GunType mType;
};

#endif // AMMOBOX_H
