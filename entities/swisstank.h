#ifndef SWISSTANK_H
#define SWISSTANK_H

#include <QPolygon>
#include "tank.h"

class SwissTank : public Tank
{
    Q_OBJECT
public:
    explicit SwissTank(QPointF pos);
    
     virtual bool  handleExplosion(Explosion *e);
     virtual void  buPaint(QPainter &p, const QRect &region);

protected:
    virtual Shoot* createShoot();

private:
    QPolygon  mCross;
};

#endif // SWISSTANK_H
