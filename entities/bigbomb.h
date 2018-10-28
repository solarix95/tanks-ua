#ifndef BIGBOMB_H
#define BIGBOMB_H

#include "entity.h"

class BigBomb : public Entity
{
    Q_OBJECT
public:
    explicit BigBomb(QPointF pos);

    virtual bool  handleExplosion(Explosion *e);
    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual QRect boundingBox() const;
    virtual void calculate(float speed, float windForce);

private slots:
    void boom();

private:
    QPolygon  mBody;
    QPolygon  mHead;
    float     mAngle;
    bool      mDestroying;
};

#endif // BIGBOMB_H
