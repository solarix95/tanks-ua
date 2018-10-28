#ifndef HAMMERSHOOT_H
#define HAMMERSHOOT_H

#include "shoot.h"

class HammerShoot : public Shoot
{
public:
    HammerShoot(Tank *t, int level, int strength = 70, int radius = 100);

protected:
    virtual void  handleGroundCollision();
    virtual void  handleEntityCollision(Entity *e);

private:
    void createChild();

    int   mLevel;
};

#endif // HAMMERSHOOT_H
