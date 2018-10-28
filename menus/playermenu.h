#ifndef PLAYERMENU_H
#define PLAYERMENU_H

#include "optionmenu.h"

class PlayerMenu : public OptionMenu
{
public:
    PlayerMenu(int playerCount = 0, bool withLocalPlayer = true, bool withNoOption = false, Menu *nextMenu = NULL);

protected:
    virtual void accept();

private:
    int mPlayerCount;

};

#endif // PLAYERMENU_H
