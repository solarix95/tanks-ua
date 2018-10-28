#include <QDebug>
#include "particle.h"

// --------------------------------------------------------------------------------
Particle::Particle(const QPointF &p, float vx, float vy, const QColor &color, Type t)
    : Entity()
{
    mType = t;
    setPos(p);
    setSpeedX(vx);
    setSpeedY(vy);
    setSolid(false);
    mColor = color;
    setWeight((mType == Soft ? 0.7 : 2.0) + (5-qrand() % 10)/10.0);
    setState(StIdle);
}

// --------------------------------------------------------------------------------
QRect Particle::boundingBox() const
{
    return QRect();
}

// --------------------------------------------------------------------------------
void Particle::buPaint(QPainter &p, const QRect &region)
{
    if (!region.contains(pos().toPoint()))
        return;
    p.save();
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(mColor);
    if (mType == Soft) {
        p.setOpacity(0.2);
        p.drawEllipse(pos(),5,5);
    } else
        p.drawEllipse(pos(),1,1);
    p.restore();
}

// --------------------------------------------------------------------------------
void Particle::handleGroundCollision()
{
    deleteLater();
}

// --------------------------------------------------------------------------------
QList<Particle *> Particle::createFountain(QPointF pos, int count, int spread, QColor col, Type t, float vyStrech)
{
    QList<Particle *> bulk;
    for (int i=0; i<count; i++) {
        float vx = (spread/2 - qrand() % spread)/10.0;
        float vy = (qrand() % spread)/(float)spread;
        if (!vy)
            vy = 0.1;
        bulk << new Particle(pos,vx,vy*vyStrech,col, t);
    }
    return bulk;
}
