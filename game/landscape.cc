#include <QtGlobal>
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <limits>
#include <stdlib.h>
#include "landscape.h"
#include "explosion.h"
#include "particle.h"
#include "tuemath.h"


#define RGB_LIMIT(x) if (x>255) x = 200;if (x<0) x=10
inline bool dblCompare(double a, double b) { return abs(a-b) <= (a*std::numeric_limits<double>::epsilon()); }

// --------------------------------------------------------------------------------
Landscape::Landscape(QObject *parent)
    : Entity(parent)
{
}

// --------------------------------------------------------------------------------
void Landscape::init(QSize size)
{
    mLandscape.clear();
    mColorGradient = QLinearGradient(QPoint(size.width()/2,0),QPoint(size.width()/2,size.height()*0.8));

    if (qrand() % 2) {
        mColorGradient.setColorAt(0.2,QColor("#555555"));
        mColorGradient.setColorAt(0.7,QColor("#AAAAAA"));
        createRockShape(size);
    } else {
        mColorGradient.setColorAt(0.2,QColor("#8f6200"));
        mColorGradient.setColorAt(0.7,QColor("#ba7f00"));
        createSandShape(size);
    }

    makePolygon();
}

// --------------------------------------------------------------------------------
void Landscape::buPaint(QPainter &p, const QRect & /*region*/)
{
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(mColorGradient);
    p.drawPolygon(mLandscape);
}

// --------------------------------------------------------------------------------
bool Landscape::collide(QPointF pos) const
{
    if (pos.x() < 0 || pos.x() >= mLandscape.size())
        return false;

    return mLandscape[pos.x()].y() > pos.y();
}

// --------------------------------------------------------------------------------
int Landscape::heightAt(int x) const
{
    Q_ASSERT(x >= 0 && x < mLandscape.size());

    return mLandscape[x].y();
}

// --------------------------------------------------------------------------------
bool Landscape::handleExplosion(Explosion *e)
{
    int where = e->pos().x();
    int myX;
    if (where < 0) {
        myX = 0;
    } else if (where >= mLandscape.size())
        myX = mLandscape.size()-1;
    else
        myX = where;
    for (int x = myX - e->radius(); x< myX+e->radius(); x++) {
        if (x<0 || x >= mLandscape.size())
            continue;
        for (int y=0; y<mLandscape.at(x).y(); y++) {
            float dist = TUE_DIST_XP(x,y,e->pos());
            if (dist <= e->radius() && (dist/(float)e->radius()*e->strength()) < 70) {
                makePepples(QPoint(mLandscape[x].x(),y),mLandscape[x].y()-y);
                mLandscape[x] = QPoint(x,y);
                break;
            }
        }
    }
    return true;
}

// --------------------------------------------------------------------------------
QRect Landscape::boundingBox() const
{
    return QRect();
}

// --------------------------------------------------------------------------------
void Landscape::createRockShape(QSize size)
{
    int faktor = 1;
    int delta  = 1;
    int min    = size.height()*0.1;
    int max    = size.height()*0.7;
    for (int i = 0; i < size.width(); i ++) {
        int last = i > 0 ? mLandscape[i-1].y(): size.height()*0.3;

        int change = abs(qrand() % 100);
        if (change > 95) {
            faktor = -faktor;
            delta   = 1 + abs(qrand() % 2);
        }

        if (last > max || last < min) {
            faktor = -faktor;
        }
        mLandscape << QPoint(i,last + (faktor * delta));
    }
}

// --------------------------------------------------------------------------------
void Landscape::createSandShape(QSize size)
{
    double delta       = (qrand() % 10 - 5)/5.0;
    double targetDelta = delta;
    double rate        = 0;
    double last        = size.height()*0.3;
    int min           = size.height()*0.2;
    int max           = size.height()*0.7;
    for (int i = 0; i < size.width(); i ++) {
        int change = abs(qrand() % 100);
        if (change > 95) {
            targetDelta   = (qrand() % 10 - 5)/5.0; // (targetDelta > 0 ? -1:1) * 1 + abs(qrand() % 3);
            float dist = sqrt(pow(delta,2)+pow(targetDelta,2));
            rate   = (targetDelta > delta ? dist:-dist)/100.0;
        }

        if (last > max)
            rate = -0.05;
        if (last < min)
            rate =  0.05;
        delta += rate;
        if (dblCompare(delta,targetDelta))
            rate = 0;
        last += delta;
        mLandscape << QPoint(i,last);
    }
}

// --------------------------------------------------------------------------------

void Landscape::makePolygon()
{
    mLandscape << QPoint(mLandscape.count()-1,0);  // right bottom
    mLandscape << QPoint(0,0);                     // left bottom
}

// --------------------------------------------------------------------------------
void Landscape::makePepples(QPoint start, int height)
{
    for (int h=0; h<height; h += 3) {
        if (qrand() % 100)
            continue;
        QList<Particle*> particles = Particle::createFountain(QPoint(start.x(),start.y()+h),2,2,Qt::gray, Particle::Hard);
        foreach(Particle *p, particles)
            emit entityCreated(p,true);

    }
}


// --------------------------------------------------------------------------------
QColor Landscape::colorAt(double pos)
{
    Q_ASSERT(pos >= 0 && pos <= 1);

    QGradientStops stops = mColorGradient.stops();
    Q_ASSERT(stops.count() > 0);
    if (stops.first().first >= pos)
        return stops.first().second;
    if (stops.last().first <= pos)
        return stops.last().second;
    for (int i=0; i<stops.count()-1; i++) {
        if (stops[i+1].first > pos) {
            int rFrom = stops[i].second.red();
            int gFrom = stops[i].second.green();
            int bFrom = stops[i].second.blue();
            int rTo   = stops[i+1].second.red();
            int gTo   = stops[i+1].second.green();
            int bTo   = stops[i+1].second.blue();
            float normF = 1.0/(stops[i+1].first - stops[i].first)*(pos-stops[i].first);
            int r = rFrom + normF*(rTo-rFrom);
            int g = gFrom + normF*(gTo-gFrom);
            int b = bFrom + normF*(bTo-bFrom);
            return QColor(r,g,b);
        }
    }

    Q_ASSERT(0);
    return QColor();
}

