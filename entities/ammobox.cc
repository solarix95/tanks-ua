#include "ammobox.h"
#include "shoot.h"
#include "tank.h"
#include "player.h"
#include "sndbackend.h"
#include "explosion.h"
#include "particle.h"
#include <QDebug>

#define SIDE  40
#define LIGHT_COLOR  QColor("#ba3d00")
#define DARK_COLOR   QColor("#632000")

// --------------------------------------------------------------------------------
AmmoBox::AmmoBox(QPointF pos, Bullet::GunType t)
    : Entity(), mType(t)
{
    setPos(pos);
    setState(StIdle);
    setEatsBullets(true);
}

// --------------------------------------------------------------------------------
QColor AmmoBox::color()
{
    static QColor lightBrown = LIGHT_COLOR;
    return lightBrown;
}

// --------------------------------------------------------------------------------
void AmmoBox::buPaint(QPainter &p, const QRect &region)
{
    static QColor darkBrown  = DARK_COLOR;
    static QColor lightBrown = LIGHT_COLOR;

    p.save();
    p.translate(pos());
    p.setPen(darkBrown);
    switch (mType) {
     case Bullet::MediumShoot:
        p.setBrush(lightBrown);
        break;
     case Bullet::ScatterShoot:
        p.setBrush(Qt::yellow);
        break;
     case Bullet::HammerShoot:
        p.setBrush(Qt::blue);
        break;
    case Bullet::AtomicShoot:
       p.setBrush(Qt::red);
       break;
    }
    p.drawRect(QRect(-SIDE/2,SIDE/2,SIDE,-SIDE));
    p.drawLine(0,SIDE/2,0,-SIDE/2);
    p.drawLine(-SIDE/2,0,SIDE/2,0);
    p.restore();
}

// --------------------------------------------------------------------------------
QRect AmmoBox::boundingBox() const
{
    return QRect(pos().x()-SIDE/2,pos().y()-SIDE/2,SIDE,SIDE);
}

// --------------------------------------------------------------------------------
void AmmoBox::calculate(float speed, float windForce)
{
    // Abwaerts, falls unter mir kein Grund mehr ist..
    bool groundCol = false;
    emit testCollision(QPoint(pos().x(),pos().y()-(SIDE*0.8)),&groundCol);
    if (!groundCol && !speedY())
        setSpeedY(0.02);

    Entity::calculate(speed,windForce);
}

// --------------------------------------------------------------------------------
void AmmoBox::hitBy(Entity *e)
{
    Shoot *s = dynamic_cast<Shoot*>(e);
    if (s && (state() == StIdle)) {
        SndBackend::instance()->play(SndBackend::WoodHit);
        if (s->tank()) {
            switch(mType) {
            case Bullet::MediumShoot:
                s->tank()->addBullets(5,Bullet::MediumShoot,60,100);
                break;
            case Bullet::ScatterShoot:
                s->tank()->addBullets(5,Bullet::ScatterShoot, 40,50);
                break;
            case Bullet::HammerShoot:
                s->tank()->addBullets(5,Bullet::HammerShoot,40,50);
                break;
            case Bullet::AtomicShoot:
                s->tank()->addBullets(1,Bullet::AtomicShoot,200,300);
                break;
            }

            QList<Particle*> particles = Particle::createFountain(pos(),30,50,QColor("#ba3d00"), Particle::Soft,5);
            foreach(Particle *p, particles)
                emit entityCreated(p,true);
            emit explosion(new Explosion(NULL,pos(),5,20));

            deleteLater();
        }
    }
}
