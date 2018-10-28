#include "smoke.h"

Smoke::Smoke()
{

}

Smoke::~Smoke()
{
    qDeleteAll(mParticles);
}

