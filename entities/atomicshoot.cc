#include <QTimer>
#include "entities/atomicshoot.h"

AtomicShoot::AtomicShoot(Tank *t, int strength, int radius)
    : Shoot(t,strength, radius)
{
    if (!(qrand() % 3))
        QTimer::singleShot(50,this,SLOT(selfDestruction()));
}

void AtomicShoot::selfDestruction()
{
    deathTrigger(true);
}
