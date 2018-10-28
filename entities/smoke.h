#ifndef SMOKE_H
#define SMOKE_H

#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QList>

class Smoke
{
public:
    Smoke();
    ~Smoke();

    void calculate(float density, QPointF pos, float speed, float windForce);
    void buPaint(QPainter &p);

private:
    int   mDensity;

    struct Particle {
        QPointF pos;
        int     opacity;
        Particle(QPointF p, int o) : pos(p), opacity(o) {}
    };

    QList<Particle*> mParticles;
};

#endif // SMOKE_H
