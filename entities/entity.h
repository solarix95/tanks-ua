#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QTime>
#include <QPointF>
#include <QRect>
#include <QPainter>

class Explosion;
class Entity : public QObject
{
    Q_OBJECT
public:
    typedef enum eState {
        StInit,
        StIdle,
        StActive,
        StDead,
        StCustomState
    } State;

    explicit Entity(QObject *parent = 0);

    inline void    setPos(QPointF p)   { mPos = p;            }
    inline QPointF pos() const         { return mPos;         }
    inline void    setSpeedX(float vx) { mSpeed.setX(vx);     }
    inline void    setSpeedY(float vy) { mSpeed.setY(vy);     }
    inline void    setWeight(float  w) { mWeight = w;         }
    inline int     state() const       { return mState;       }
    inline bool    isSolid() const     { return mIsSolid;     }
    inline bool    eatsBullets() const { return mEatsBullets; }
    inline float   speedY()  const     { return mSpeed.y();   }
    inline float   speedX()  const     { return mSpeed.x();   }
    inline int     health()  const     { return mHealth;      }
    inline int     id() const          { return mId;          }
    inline int     timeInState() const { return mStateTime.elapsed(); }

    virtual void  tdPaint(QPainter &p, const QSize &size);
    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual void  calculate(float speed, float windForce);
    virtual bool  handleExplosion(Explosion *e);
    virtual void  handleDeath();
    virtual void  decHealth(int d, Entity *source);
    virtual QRect boundingBox() const = 0;

    static bool calcStep(const QPointF &oldPos, QPointF &newPos, QPointF &speed, float gameSpeed, float weight, float windForce = 0);

signals:
    void setCamera(int x, int behaviour);
    void testCollision(QPointF p, bool *withGround);
    void testCollision(Entity *, Entity **withEntity);
    void explosion(Explosion *e);
    void requestKill(Entity *target);
    void entityCreated(Entity *e, bool prepend);
    
public slots:

protected:
    inline void setSolid(bool s)      { mIsSolid = s;     }
    inline void setEatsBullets(bool e){ mEatsBullets = e; }
    inline void setHealth(int h)      { mHealth  = h;     }

    virtual void  setState(int s);
    virtual void  handleGroundCollision();
    virtual void  handleEntityCollision(Entity *e);
    virtual void  handleStateChange();
    virtual void  hitBy(Entity *e);

private:
    int        mId;
    int        mHealth;
    QPointF    mPos;
    QPointF    mSpeed;
    float      mWeight;

    int        mState;
    bool       mIsSolid;
    bool       mEatsBullets;
    QTime      mStateTime;
    static int sId;
};

#endif // ENTITY_H
