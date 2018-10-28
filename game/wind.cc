#include <QtGlobal>
#include <QDebug>
#include "wind.h"
#include <math.h>

#define  F_MAX    10
#define  D_MAX     3

Wind::Wind()
{
    mForce    = (qrand() % (2*F_MAX+1)) - F_MAX; // -3 .. +3
    mDelta    = (qrand() % (2*D_MAX+1)) - D_MAX;
    mGradient = (qrand() % (2*D_MAX+1)) - D_MAX;
}

void Wind::calculate(float speed)
{
    float  oldForce = mForce;
    mForce += (mDelta*speed)/200;

    float newDelta =  mDelta + mGradient/100;
    if (newDelta < -D_MAX)
        newDelta = -D_MAX;
    if (newDelta >  D_MAX)
        newDelta =  D_MAX;

    // Nulldurchgang?
    int posNeg1 = oldForce / fabs(oldForce);
    int posNeg2 = mForce   / fabs(mForce);
    if (posNeg1 != posNeg2) { // Nulldurchgang, z.B: 0.03 -> -0.02
        mGradient = (qrand() % (2*D_MAX+1)) - D_MAX;
    }

    if (mForce < -F_MAX)
        mGradient = +((qrand()%D_MAX) + 1);
    if (mForce > +F_MAX)
        mGradient = -((qrand()%D_MAX) + 1);

    mDelta = newDelta;
}
