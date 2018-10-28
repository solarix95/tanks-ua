#ifndef ROUNDSMENU_H
#define ROUNDSMENU_H

#include "optionmenu.h"

class RoundsMenu : public OptionMenu
{
public:
    RoundsMenu(Menu *next = NULL);

protected:
    virtual void accept();
    virtual void reject();
};

#endif // ROUNDSMENU_H
