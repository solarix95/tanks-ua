#ifndef PARTICLE_H
#define PARTICLE_H

#include <QColor>
#include <QPoint>
#include <QList>
#include "entity.h"

class Particle : public Entity
{
public:
    typedef enum eType {
        Soft,   // Baumblätter, Schnee, Regen
        Hard    // Kies
    } Type;

    Particle(const QPointF &p, float vx, float vy, const QColor &color, Type t);

    virtual QRect boundingBox() const;
    virtual void  buPaint(QPainter &p, const QRect &region);
    virtual void  handleGroundCollision();

    static QList<Particle*> createFountain(QPointF pos, int count, int spread, QColor col, Type t = Soft, float vyStrech = 1.0);

private:
    QColor mColor;
    Type   mType;
};

#endif // PARTICLE_H
