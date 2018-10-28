#include "sky.h"

Sky::Sky(QObject *parent)
    : Entity(parent)
{
    mPlanet.load("earth.png");
}

void Sky::tdPaint(QPainter &p, const QSize &size)
{
    p.save();
    QLinearGradient sky(0,0,0,size.height());

    /*
    sky.setColorAt(0.2,QColor(0,0,200));
    sky.setColorAt(0.5,QColor(0,0,100));
    */

    sky.setColorAt(0.1,QColor(0,0,200));
    sky.setColorAt(0.8,QColor(90,0,110));
    sky.setSpread(QGradient::PadSpread);

    p.setBrush(sky);
    p.fillRect(0,0,size.width(),size.height(),sky);

    // p.setOpacity(0.3);
    // p.drawPixmap(50,50,mPlanet);
    p.restore();
}


QRect Sky::boundingBox() const
{
    return QRect();
}
