#ifndef SKY_H
#define SKY_H

#include <QPixmap>
#include "entity.h"

class Sky : public Entity
{
public:
    Sky(QObject *parent);

    virtual void tdPaint(QPainter &p, const QSize &size);

protected:
    virtual QRect boundingBox() const;

private:
    QPixmap   mPlanet;
};

#endif // SKY_H
