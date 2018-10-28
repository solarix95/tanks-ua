#include <math.h>
#include "swisstank.h"
#include "explosion.h"
#include "shoot.h"

// --------------------------------------------------------------------------------
SwissTank::SwissTank(QPointF pos) :
    Tank(pos)
{
    setPlayable(false); // wird vom Game ignoriert
    setHealth(300);
    setColor(Qt::gray);
    mCross << QPoint(-1,+1);
    mCross << QPoint(-1,+3);
    mCross << QPoint(+1,+3);
    mCross << QPoint(+1,+1);
    mCross << QPoint(+3,+1);
    mCross << QPoint(+3,-1);
    mCross << QPoint(+1,-1);
    mCross << QPoint(+1,-3);
    mCross << QPoint(-1,-3);
    mCross << QPoint(-1,-1);
    mCross << QPoint(-3,-1);
    mCross << QPoint(-3,+1);
}

// --------------------------------------------------------------------------------
bool SwissTank::handleExplosion(Explosion *e)
{
    if (Tank::handleExplosion(e)) {
        if (health() > 0 && e->tank() && (e->tank() != this)) {
            if (!(qrand() % 5))
                emit requestKill(e->tank()); // Wolke herbeizitieren
            else {
                // Rache ist süss...
                aimBot(e->tank()->pos());
                fire();
            }
        }
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
void SwissTank::buPaint(QPainter &p, const QRect &region)
{
    Tank::buPaint(p,region);
    p.save();

    p.translate(pos().x(),pos().y()+10);
    p.scale(2,2);

    p.setBrush(Qt::red);
    p.drawRect(-4,-4,8,8);
    p.setBrush(Qt::white);
    p.drawPolygon(mCross);

    p.restore();
}

// --------------------------------------------------------------------------------
Shoot *SwissTank::createShoot()
{
    return new Shoot(this,200,300);
}
