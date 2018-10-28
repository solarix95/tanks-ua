#include <QDebug>
#include "cloud.h"
#include "shoot.h"
#include "tank.h"
#include "camera.h"
#include "sndbackend.h"
#include "particle.h"

// --------------------------------------------------------------------------------
Cloud::Cloud(QPoint pos)
    : Entity()
{
    mLastHitEntity = 0;
    mMaxHits       = 1 + qrand() % 4;
    mHitCount      = 0;
    setPos(pos);
    mCloudImage.load(":images/cloud.png");
    mCloudImage = mCloudImage.scaled(140,80);
    setState(StIdle);
    setSolid(false);
    connect(&mRainTimer, SIGNAL(timeout()), this, SLOT(letItRain()));
    mRainTimer.start(7000 + (qrand()%10000));
}

// --------------------------------------------------------------------------------
void Cloud::buPaint(QPainter &p, const QRect &region)
{
    // render Lightning
    if (state() == StDestroyTarget) {
        paintLightning(p);
    }

    // render Cloud
    p.save();
    p.setOpacity(0.5);
    p.translate(-mCloudImage.width()/2,-mCloudImage.height()/2);
    p.drawPixmap(pos().x(),pos().y(),mCloudImage);
    p.restore();
}

// --------------------------------------------------------------------------------
void Cloud::calculate(float speed,float wind)
{
    switch(state()) {
      case StMoveToTarget :
        Q_ASSERT(mTarget);
        if (abs(mTarget->pos().x() - pos().x()) < 20) {
            setState(StAboutToDestroyTarget);
            emit setCamera(pos().x(),Camera::StartEvent);
        } else {
            float diff = speed*(mTarget->pos().x()-pos().x())/30.0;
            setPos(QPointF(pos().x() + diff,pos().y()));
            emit setCamera(pos().x(),Camera::Soft);
        }
        break;
      case StAboutToDestroyTarget :
         SndBackend::instance()->play(SndBackend::ElectricZap);
         setState(StDestroyTarget);
         break;
      case StDestroyTarget :
        Q_ASSERT(mTarget);
        mTarget->decHealth(1,this);
        break;
      default:
        setPos(QPointF(pos().x() + (speed*wind)/10,pos().y()));
    }
}

// --------------------------------------------------------------------------------
void Cloud::destroyTarget(Entity *e)
{
    mTarget = e;
    Q_ASSERT(mTarget);
    connect(mTarget, SIGNAL(destroyed(QObject*)), this, SLOT(tankDestroyed(QObject*)));
    setState(StMoveToTarget);
}

// --------------------------------------------------------------------------------
void Cloud::handleStateChange()
{
}

// --------------------------------------------------------------------------------
QRect Cloud::boundingBox() const
{
    int w = mCloudImage.width();
    int h = mCloudImage.height();
    return QRect(pos().x() - w/2,pos().y() - h/2,w,h);
}

// --------------------------------------------------------------------------------
void Cloud::hitBy(Entity *e)
{
    Shoot *s = dynamic_cast<Shoot*>(e);
    if (s && e->id() != mLastHitEntity && (state() == StIdle)) {
        QList<Particle*> particles = Particle::createFountain(pos(),10,20,QColor(Qt::white));
        foreach(Particle *p, particles)
            emit entityCreated(p,true);
        mLastHitEntity = e->id();
        if (++mHitCount >= mMaxHits) {
            destroyTarget(s->tank());

        }
    }
}

// --------------------------------------------------------------------------------
void Cloud::tankDestroyed(QObject *t)
{
    if (t == mTarget)
        reset();
}

// --------------------------------------------------------------------------------
void Cloud::letItRain()
{
    if (qrand() % 10 > 7) {
        QList<Particle*> particles = Particle::createFountain(pos(),20,30,QColor(Qt::blue));
        foreach(Particle *p, particles)
            emit entityCreated(p,true);
    }
}

// --------------------------------------------------------------------------------
void Cloud::paintLightning(QPainter &p)
{
    Q_ASSERT(mTarget);
    p.save();

    // create random yellow:
    QColor lColor(qMax(qMin(QColor(Qt::yellow).red()   + (125-qrand()%250),255),0),
                  qMax(qMin(QColor(Qt::yellow).green() + (125-qrand()%250),255),0),
                  qMax(qMin(QColor(Qt::yellow).blue()  + (125-qrand()%250),255),0));

    p.setPen(QPen(lColor,5,Qt::SolidLine,Qt::RoundCap));
    int mx = pos().x();
    int my = pos().y();
    int tx = mTarget->pos().x();
    int ty = mTarget->pos().y() + 10;
    QPointF mid(mx + (tx-mx)/2,my - (my-ty)/2);
    QPointF left(mid.x()-10,mid.y()+10);
    QPointF right(mid.x()+10,mid.y()-10);
    QVector<QPointF> lightning;
    lightning << pos();
    lightning << right << right;
    lightning << left  << left;
    lightning << QPointF(tx,ty);
    p.drawLines(lightning);
    p.restore();
}

// --------------------------------------------------------------------------------
void Cloud::reset()
{
    mTarget   = NULL;
    mHitCount = 0;
    setState(StIdle);
    emit setCamera(-1,Camera::EndEvent);
    SndBackend::instance()->play(SndBackend::Thunder);
}
