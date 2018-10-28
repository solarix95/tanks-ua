#ifndef SCATTERSHOOT_H
#define SCATTERSHOOT_H

#include "shoot.h"

class ScatterShoot : public Shoot
{
public:
    ScatterShoot(Tank *t, int strength = 70, int radius = 100);

    virtual void  calculate(float speed, float windForce);

private:
    void createChilds();
    void createChild(int index);
};

#endif // SCATTERSHOOT_H
