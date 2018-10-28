#ifndef ATOMICSHOOT_H
#define ATOMICSHOOT_H

#include "shoot.h"

class AtomicShoot : public Shoot
{
    Q_OBJECT
public:
    AtomicShoot(Tank *t, int strength = 70, int radius = 100);

private slots:
    void selfDestruction();
};

#endif // ATOMICSHOOT_H
