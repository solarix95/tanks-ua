#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "entity.h"

class Tank;
class Explosion : public Entity
{
public:
    Explosion(Tank *t, QPointF pos, int strength, int radius);

    void setColors(QColor from, QColor to);

    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual void  calculate(float speed, float windForce);
    virtual QRect boundingBox() const;

    inline int   strength() const { return mStrength; }
    inline int   radius()   const { return mRadius;   }
    inline Tank* tank()     const { return mTank;     }

private:
    int     mStrength;
    int     mRadius;
    Tank   *mTank;
    float   mLifeTime;

    // Animation:
    QColor  mStartColor;
    float   mDeltaRed;
    float   mDeltaGreen;
    float   mDeltaBlue;
};

#endif // EXPLOSION_H
