#ifndef CLOUD_H
#define CLOUD_H

#include <QTimer>
#include "entity.h"

class Shoot;
class Tank;
class Cloud : public Entity
{
    Q_OBJECT
public:
    Cloud(QPoint pos);

    virtual void buPaint(QPainter &p, const QRect &region);
    virtual void calculate(float speed, float wind);

    void destroyTarget(Entity *e);

protected:
    virtual void  handleStateChange();
    virtual QRect boundingBox() const;
    virtual void  hitBy(Entity *e);

private slots:
    void tankDestroyed(QObject *t);
    void letItRain();

private:
    void paintLightning(QPainter &p);
    void reset();

    typedef enum ClState_ {
        StMoveToTarget = Entity::StCustomState,
        StAboutToDestroyTarget,
        StDestroyTarget
    } ClState;

    QPixmap  mCloudImage;
    int      mLastHitEntity;
    Entity  *mTarget;
    int      mHitCount;
    int      mMaxHits;

    QTimer   mRainTimer;
};

#endif // CLOUD_H
