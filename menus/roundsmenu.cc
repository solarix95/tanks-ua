#include "roundsmenu.h"
#include "gameoption.h"
#include "playermenu.h"
#include "featuremenu.h"

// --------------------------------------------------------------------------------
RoundsMenu::RoundsMenu(Menu *next)
    : OptionMenu("rounds",false,next)
{
    addOption(QImage(":images/round-1.png"),GameOption(2,"2"));
    addOption(QImage(":images/round-2.png"),GameOption(4,"4"));
    addOption(QImage(":images/round-3.png"),GameOption(8,"8"));
    addOption(QImage(":images/round-4.png"),GameOption(16,"16"));
}

// --------------------------------------------------------------------------------
void RoundsMenu::accept()
{
    OptionMenu::accept();
    emit activate(new PlayerMenu());
}

// --------------------------------------------------------------------------------
void RoundsMenu::reject()
{
    emit activate(new FeatureMenu());
}
