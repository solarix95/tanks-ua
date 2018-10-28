#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QList>
#include <QColor>
#include <QPolygon>
#include "entity.h"


class QPainter;
class Explosion;
class Landscape : public Entity
{
public:
    Landscape(QObject *parent);

    void init(QSize size);

    virtual void buPaint(QPainter &p, const QRect &region);

    bool collide(QPointF pos) const;
    int  heightAt(int x) const;
    void boundingBox(Explosion *e);
    virtual bool handleExplosion(Explosion *e);

protected:
    virtual QRect boundingBox() const;

private:
    void createRockShape(QSize size);
    void createSandShape(QSize size);
    void makePolygon();
    void makePepples(QPoint start, int height);

    QColor colorAt(double pos); // 0..1

    QLinearGradient mColorGradient;
    QPolygon        mLandscape;
};

#endif // LANDSCAPE_H
