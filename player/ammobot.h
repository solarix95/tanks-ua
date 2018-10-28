#ifndef AMMOBOT_H
#define AMMOBOT_H

#include "bot.h"

class AmmoBot : public Bot
{
    Q_OBJECT
public:
    AmmoBot(const QString &name);
    virtual void setTank(Tank *t);
    virtual void handleNewEnemy();
    virtual void handleAmmoBoxes();

protected:
    virtual void handleStateChange();

private slots:
    void gotBullets();

private:
    void huntTank();
    void huntAmmo();

    enum AmmoBotState {
        ABInit,        // Wähle AmmoBox oder gegn. Tank
        ABHuntAmmo,    // Suche AmmoBox
        ABHuntTarget   // Suche gegn. Tank
    };

    void setBotState(AmmoBotState st);

    AmmoBotState mBotState;
};

#endif // AMMOBOT_H
