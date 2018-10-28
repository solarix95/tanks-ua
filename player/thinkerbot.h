#ifndef THINKERBOT_H
#define THINKERBOT_H

#include "bot.h"

class ThinkerBot : public Bot
{
    Q_OBJECT
public:
    ThinkerBot(const QString &name);
    virtual void setTank(Tank *t);

protected:
    virtual void handleStateChange();


private:
};

#endif // THINKERBOT_H
