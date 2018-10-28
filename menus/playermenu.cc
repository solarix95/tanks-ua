#include "gameoption.h"
#include "playermenu.h"

// --------------------------------------------------------------------------------
PlayerMenu::PlayerMenu(int playerCount, bool withLocalPlayer, bool withNoOption, Menu *nextMenu)
    : OptionMenu(QString("player-%1").arg(playerCount),withNoOption,nextMenu)
{
    mPlayerCount = playerCount;
    if (withLocalPlayer)
        addOption(QImage(":images/player_local.png"),GameOption(GameOption::LocalPlayer,"you"));
    addOption(QImage(":images/player_anarki.png"),GameOption(GameOption::Anarki,"Anarki")); // green Tank
    addOption(QImage(":images/player_nerd.png"),GameOption(GameOption::AngryDuke,"Nerd"));  // Yellow Tank
    addOption(QImage(":images/player_iceman.png"),GameOption(GameOption::Iceman,"Iceman"));   // Red Tank
}

// --------------------------------------------------------------------------------
void PlayerMenu::accept()
{
    OptionMenu::accept();
    if (currentOption().option() == GameOption::NoOption) {
        emit startGame();
        return;
    }

    if (mPlayerCount == 0)
        emit activate(new PlayerMenu(mPlayerCount+1,false));
    else
        emit activate(new PlayerMenu(mPlayerCount+1,false,true));
}
