#ifndef VENDETTABOT_H
#define VENDETTABOT_H

#include "bot.h"

class Tank;
class VendettaBot : public Bot
{
    Q_OBJECT
public:
    VendettaBot(const QString &name);

    virtual void setTank(Tank *t);

protected:
    virtual void handleStateChange();

private slots:
    void hitBy(Tank*);

};

#endif // VENDETTABOT_H
