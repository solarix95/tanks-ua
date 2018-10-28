#include <QDebug>
#include "explosion.h"
#include "sndbackend.h"

#define LIFE  10.0

// --------------------------------------------------------------------------------
Explosion::Explosion(Tank *t, QPointF pos, int strength, int radius)
    : Entity()
{
    mStrength = strength;
    mRadius   = radius;
    mTank     = t;
    mLifeTime = 0;
    setPos(pos);
    setColors(QColor("#CC0000"),QColor("#FFCC33")); // red -> orange

    if (mStrength > 100)
        SndBackend::instance()->play(SndBackend::MediumBoom);
    else
        SndBackend::instance()->play(SndBackend::LightBoom);
}

// --------------------------------------------------------------------------------
void Explosion::setColors(QColor from, QColor to)
{
    mStartColor  = from;
    mDeltaRed    = (to.red()-from.red())/LIFE;
    mDeltaGreen  = (to.green()-from.green())/LIFE;
    mDeltaBlue   = (to.blue()-from.blue())/LIFE;
}

// --------------------------------------------------------------------------------
void Explosion::buPaint(QPainter &p, const QRect &region)
{
     p.save();
     p.translate(pos().x()-mRadius,pos().y()-mRadius);

     QColor c = QColor(qMin(mStartColor.red()  + mLifeTime*mDeltaRed,(float)255),
                       qMin(mStartColor.green()+ mLifeTime*mDeltaGreen,(float)255),
                       qMin(mStartColor.blue() + mLifeTime*mDeltaBlue,(float)255));
     p.setPen(c);
     p.setBrush(c);
     p.drawEllipse(0,0,2*mRadius,2*mRadius);
     p.restore();
}

// --------------------------------------------------------------------------------
void Explosion::calculate(float speed, float /*windForce*/)
{
    mLifeTime += speed;
    if (mLifeTime > LIFE)
        deleteLater();
}

// --------------------------------------------------------------------------------
QRect Explosion::boundingBox() const
{
    return QRect();
}
