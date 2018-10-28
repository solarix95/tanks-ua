#include "tree.h"
#include "shoot.h"
#include "tank.h"
#include "particle.h"
#include "sndbackend.h"
#include "math.h"

#define SCALE  5

// --------------------------------------------------------------------------------
Tree::Tree(QPoint pos)
    : Entity()
{
    setPos(pos);
    setEatsBullets(true);

    mTree << QPoint( -4,  0);
    mTree << QPoint( -2, +2);
    mTree << QPoint( -1,+10);
    mTree << QPoint( +1,+10);
    mTree << QPoint( +2, +2);
    mTree << QPoint( +4,  0);

    for (int i=0; i<45; i++)
        mLeafs << randomLeaf();
}


// --------------------------------------------------------------------------------
void Tree::buPaint(QPainter &p, const QRect &region)
{
    p.save();
    p.translate(pos());
    p.setBrush(QColor("#660000")); // Brown
    p.scale(SCALE,SCALE);
    p.drawPolygon(mTree);
    p.translate(0,15);
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(QColor(Qt::green).darker());
    foreach(QPoint l, mLeafs)
        p.drawEllipse(l,2,2);
    p.restore();
}

// --------------------------------------------------------------------------------
void Tree::calculate(float speed, float windForce)
{
    // Abwaerts, falls unter mir kein Grund mehr ist..
    bool groundCol = false;
    emit testCollision(QPoint(pos().x(),pos().y()),&groundCol);
    if (!groundCol && !speedY())
        setSpeedY(0.01);

    Entity::calculate(speed,windForce);
}

// --------------------------------------------------------------------------------
void Tree::handleGroundCollision()
{
    setState(StIdle);
    Entity::handleGroundCollision();
}

// --------------------------------------------------------------------------------
bool Tree::handleExplosion(Explosion *e)
{
    if (Entity::handleExplosion(e)) {
        QPointF leafPos(pos().x(),pos().y()+15);
        QList<Particle*> particles = Particle::createFountain(leafPos,10,100,QColor(Qt::green));
        foreach(Particle *p, particles)
            emit entityCreated(p,true);
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------------
void Tree::hitBy(Entity *e)
{
    Shoot *s = dynamic_cast<Shoot*>(e);
    if (s && (state() == StIdle)) {
        SndBackend::instance()->play(SndBackend::WoodHit);
        if (!(qrand() % 3))
            emit requestKill(s->tank());
    }
}

// --------------------------------------------------------------------------------
QRect Tree::boundingBox() const
{
    return QRect(pos().x()-2*SCALE,pos().y(),4*SCALE,20*SCALE);
}

// --------------------------------------------------------------------------------
QPoint Tree::randomLeaf() const
{
    float radius = qrand() % 6;
    float angle  = qrand() % 360;
    float x = cos(angle/180.0*(3.1415)) * radius;
    float y = sin(angle/180.0*(3.1415)) * radius;
    return QPoint(x,y);
}
