#ifndef TANK_H
#define TANK_H

#include <QPointF>
#include "entity.h"
#include "bullet.h"

class Shoot;
class Tank : public Entity
{
    Q_OBJECT
public:
    Tank(QPointF pos);

    void   incAngle(int da);
    Shoot* fire();

    inline void   setAngle(float a)  { mAngle = ((int)a)%360;  }
    inline float  angle() const      { return mAngle;          }
    inline void   setForce(float f)  { mForce = qMin(f,(float)1000);  }
    inline float  force() const      { return mForce;          }

    inline bool   isPlayable() const { return mIsPlayable; }
    inline void   setSelected(bool s){ mIsSelected = s;    }
    inline void   setColor(QColor c) { mColor = c;         }
    inline QColor color() const      { return mColor;      }

    inline void   setIsLeader(bool l){ mIsLeader = l;      }

    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual void  calculate(float speed, float windForce);
    virtual QRect boundingBox() const;
    virtual void  handleDeath();
    virtual void  decHealth(int d, Entity *source);
    virtual void  addPlayerPoints(int pts);
    virtual void  youKilledMe();
    virtual void  addBullets(int count, Bullet::GunType t, int strength, int radius);

    void aimBot(QPointF ePos);

signals:
    void shoot(Shoot *s);
    void playerPoints(int pts); // für PlayerStatistik
    void kills();               // für PlayerStatistik
    void tankHit(Tank*);        // für Bots (VendettaBot)
    void bulletsAdded();        // für Bots (AmmoBot)

protected:
    inline void    setPlayable(bool p)    { mIsPlayable = p; }
    virtual void   handleGroundCollision();
    virtual void   hitBy(Entity *e);
    virtual Shoot* createShoot();

private:
    float    mAngle;
    float    mForce;
    QColor   mColor;
    bool     mIsPlayable; // false -> swisstank
    bool     mIsSelected;

    QList<Bullet>  mBullets;
    bool           mIsLeader;
    QPolygon       mKing;
};

#endif // TANK_H
