#ifndef TREE_H
#define TREE_H

#include <QPolygon>
#include <QList>
#include <QPoint>
#include "entity.h"

class Tree : public Entity
{
    Q_OBJECT
public:
    Tree(QPoint pos);

    virtual void buPaint(QPainter &p, const QRect &region);
    virtual void calculate(float speed, float windForce);
    virtual void handleGroundCollision();
    virtual bool handleExplosion(Explosion *e);
    virtual void hitBy(Entity *e);

protected:
    virtual QRect boundingBox() const;

private:
    QPoint randomLeaf() const;

    QPolygon        mTree;
    QList<QPoint>   mLeafs;
};

#endif // TREE_H
