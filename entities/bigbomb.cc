#include <QTimer>
#include "bigbomb.h"
#include "explosion.h"
#include "particle.h"

#define SCALE  5

// --------------------------------------------------------------------------------
BigBomb::BigBomb(QPointF pos)
    : Entity()
{


    setPos(pos);
    setState(StIdle);

    // Kopf
    mHead << QPoint( 3, 2);
    mHead << QPoint( 4, 1);
    mHead << QPoint( 4,-1);
    mHead << QPoint( 3,-2);
    mHead << QPoint( 3, 2);

    // "Flossen"
    mBody << QPoint( 3,-2);
    mBody << QPoint(-3,-2);
    mBody << QPoint(-5,-5);
    mBody << QPoint(-5, 5);
    mBody << QPoint(-3, 2);
    mBody << QPoint( 3, 2);
    mBody << QPoint( 3,-2);

    mAngle      = qrand();
    mDestroying = false;
}

// --------------------------------------------------------------------------------
bool BigBomb::handleExplosion(Explosion *e)
{
    if (Entity::handleExplosion(e)) {
        if (!mDestroying) {
            mDestroying = true;
            setState(StActive);
            QTimer::singleShot(qrand() % 500,this, SLOT(boom()));
        }
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
void BigBomb::buPaint(QPainter &p, const QRect &region)
{
    p.save();

    p.translate(pos());
    p.scale(SCALE,SCALE);
    p.rotate(mAngle);
    p.setPen(QPen(Qt::black,1.0/SCALE));
    p.setBrush(QColor(Qt::darkGray).darker());
    p.drawPolygon(mBody);
    p.drawLine(-5,0,-4,0);
    p.drawLine(-4,2,-4,-2);

    p.setBrush(Qt::red);
    p.drawPolygon(mHead);

    p.restore();
}

// --------------------------------------------------------------------------------
QRect BigBomb::boundingBox() const
{
    return QRect();
}


// --------------------------------------------------------------------------------
void BigBomb::calculate(float speed, float windForce)
{
    // Abwaerts, falls unter mir kein Grund mehr ist..
    bool groundCol = false;
    emit testCollision(QPoint(pos().x(),pos().y()),&groundCol);
    if (!groundCol && !speedY())
        setSpeedY(0.02);

    Entity::calculate(speed,windForce);
}

// --------------------------------------------------------------------------------
void BigBomb::boom()
{
    Q_ASSERT(mDestroying);
    emit explosion(new Explosion(NULL,pos(),400,400));
    QList<Particle*> particles = Particle::createFountain(pos(),12,50,Qt::red, Particle::Soft,20);
    foreach(Particle *p, particles)
        emit entityCreated(p,true);

    deleteLater();
}
