
#include <QDebug>
#include <math.h>
#include "tank.h"
#include "sndbackend.h"
#include "camera.h"
#include "shoot.h"
#include "hammershoot.h"
#include "scattershoot.h"
#include "atomicshoot.h"
#include "explosion.h"

#define SIZE 50

// --------------------------------------------------------------------------------
Tank::Tank(QPointF pos)
    : Entity()
{
    setPos(pos);
    setSpeedY(0.01);
    mAngle      = 90 + ((qrand()%90)-45);
    mForce      = 40;
    mColor      = Qt::blue;
    mIsPlayable = true;
    mIsSelected = false;
    mIsLeader   = false;
    setHealth(100);

    mKing << QPoint(  0,  0);
    mKing << QPoint(  2, -1);
    mKing << QPoint(  4,  5);
    mKing << QPoint(  1,  3);
    mKing << QPoint(  0,  4);
    mKing << QPoint( -1,  3);
    mKing << QPoint( -4,  5);
    mKing << QPoint( -2, -1);
    mKing << QPoint(  0,  0);

    for (int i=0; i<100; i++)
        mBullets << Bullet(Bullet::StandardShoot,20,50);
}

// --------------------------------------------------------------------------------
void Tank::buPaint(QPainter &p, const QRect &region)
{
    p.save();    
    p.translate(pos());
    p.setPen(mIsSelected && (state() != StDead) ? QPen(Qt::white,2):QPen());
    p.setBrush(state() == StDead ? Qt::black : mColor);
    p.save();
    p.rotate(mAngle);
    p.drawRect( QRect(4, 0, 40, 4) );
    p.drawRect( QRect(4, -4, 40, 4) );
    p.restore();
    p.drawPie( QRect(-SIZE/2,-SIZE/2, SIZE, SIZE), 180*16, 180*16 );
    if (mIsLeader) {
        p.setPen(QPen());
        p.setBrush(Qt::yellow);
        p.translate(0,SIZE/2);
        p.scale(3,3);
        p.drawPolygon(mKing);
    }
    p.restore();
}

// --------------------------------------------------------------------------------
void Tank::calculate(float speed, float windForce)
{
    // Abwaerts, falls unter mir kein Grund mehr ist..
    bool groundCol = false;
    emit testCollision(QPoint(pos().x(),pos().y()),&groundCol);
    if (!groundCol && !speedY())
        setSpeedY(0.01);

    Entity::calculate(speed,windForce);
}

// --------------------------------------------------------------------------------
QRect Tank::boundingBox() const
{
    return QRect(pos().x() - SIZE/2,pos().y() - SIZE/2,SIZE,SIZE);
}

// --------------------------------------------------------------------------------
void Tank::handleDeath()
{
    Entity::handleDeath();
    SndBackend::instance()->play(SndBackend::Death);
    QTimer::singleShot(3000,this,SLOT(deleteLater()));
}

// --------------------------------------------------------------------------------
void Tank::handleGroundCollision()
{
    setState(StIdle);
    Entity::handleGroundCollision();
}

// --------------------------------------------------------------------------------
void Tank::hitBy(Entity *e)
{
    Shoot *s = dynamic_cast<Shoot*>(e);
    if (s) {
        SndBackend::instance()->play(SndBackend::MetallicHit);
        setSpeedY(5); // little jump
        if (s->tank())
            emit tankHit(s->tank());
    }
}

// --------------------------------------------------------------------------------
Shoot *Tank::createShoot()
{
    Bullet b = mBullets.takeLast();
    // return new ScatterShoot(this, b.strength, b.radius);

    Shoot *ret = NULL;
    switch (b.type) {
      case Bullet::StandardShoot:
      case Bullet::MediumShoot:
        ret = new Shoot(this, b.strength, b.radius);
        break;
      case Bullet::ScatterShoot:
        ret = new ScatterShoot(this, b.strength, b.radius);
        break;
      case Bullet::HammerShoot:
        ret = new HammerShoot(this, 3, b.strength, b.radius);
       break;
        case Bullet::AtomicShoot:
        ret = new AtomicShoot(this, b.strength, b.radius);
        break;
    }

    Q_ASSERT(ret);
    return ret;
}

// --------------------------------------------------------------------------------
void Tank::incAngle(int da)
{
    mAngle += da;
}

// --------------------------------------------------------------------------------
Shoot* Tank::fire()
{
    Q_ASSERT(health() > 0);
    Shoot *s = createShoot();
    emit shoot(s);
    emit setCamera(pos().x(),Camera::Hard);

    // Berechne Startpunkt (Kanonenrohr..)
    float radAngle1 = (mAngle > 90 ? 180-mAngle : mAngle)/360*2*3.1415;
    float radAngle2 = mAngle/360*2*3.1415;
    float sh = sin(radAngle1)*SIZE;
    float ch = cos(radAngle2)*SIZE;
    QPointF bulletPos = pos();
    bulletPos.setX(pos().x()+ch);
    bulletPos.setY(pos().y()+sh);

    SndBackend::instance()->play(SndBackend::LightShoot);
    s->fire(bulletPos,mAngle,mForce);
    return s;
}

// --------------------------------------------------------------------------------
void Tank::decHealth(int d, Entity *source)
{
    if (health() <= 0)
        return;

    int oldHealth = health();
    Entity::decHealth(d,source);

    int decMaxHealth = oldHealth-health();
    addPlayerPoints(-decMaxHealth);
    Explosion *e = dynamic_cast<Explosion*>(source);
    if (e) {
        if (e->tank() && (e->tank() != this)) {
            e->tank()->addPlayerPoints(decMaxHealth);
            if (health() <= 0) {
                e->tank()->addPlayerPoints(300); // Kill-Bonus
                e->tank()->youKilledMe();
            }
        }
    }
}

// --------------------------------------------------------------------------------
void Tank::addPlayerPoints(int pts)
{
    emit playerPoints(pts);
}

// --------------------------------------------------------------------------------
void Tank::youKilledMe()
{
    emit kills();
}

// --------------------------------------------------------------------------------
void Tank::addBullets(int count, Bullet::GunType t, int strength, int radius)
{
    for (int i=0; i<count; i++)
        mBullets << Bullet(t, strength, radius);
    emit bulletsAdded();
}

// --------------------------------------------------------------------------------
void Tank::aimBot(QPointF ePos)
{
    bool isRight = ePos.x() > pos().x() ? true : false;
    int randAngle = qrand() % 7;
    setAngle((isRight ? 70 : 110) + randAngle);

    int nextForce = 10;
    int lastDist = 999999;
    int currentDist;
    bool stop = false;

    while (!stop) {
        QPointF nextPos = pos();
        nextPos.setY(nextPos.y() + 10);
        float vx = cos(angle()/180.0*(3.1415)) * nextForce;
        float vy = sin(angle()/180.0*(3.1415)) * nextForce;
        QPointF speed(vx,vy);
        do {
            calcStep(nextPos,nextPos,speed,5,3);
        } while (nextPos.y() > 0);

        currentDist = sqrt(pow(nextPos.x() - ePos.x(),2));
        if (currentDist > lastDist)
            stop = true;
        else {
            nextForce++;
            lastDist = currentDist;
        }
    }

    setForce(nextForce*1.06); // Weil jeweils mit Bodentreffer gerechnet wird (nextPos.y() > 0) muss etwas weiter geballert werden..
}
