#ifndef TUEMATH_H
#define TUEMATH_H

#include <math.h>

#define TUE_DIST_P(p1,p2)   sqrt(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2))
#define TUE_DIST_XP(X,Y,p)  sqrt(pow(X-p.x(),2)+pow(Y-p.y(),2))

#endif
