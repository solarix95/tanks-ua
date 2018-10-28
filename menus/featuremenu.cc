#include "featuremenu.h"
#include "gameoption.h"
#include "roundsmenu.h"
#include "infomenu.h"

FeatureMenu::FeatureMenu(Menu *next)
    : OptionMenu("features",false,next)
{

    addOption(QImage(":images/feature1.png"),GameOption(GameOption::Features(GameOption::TanksOnly),"Classic Tanks"));
    addOption(QImage(":images/feature2.png"),GameOption(GameOption::Features(GameOption::WithEnvironment),"Unexpected Enemies"));
    addOption(QImage(":images/feature3.png"),GameOption(GameOption::Features(GameOption::WithEnvironment | GameOption::WithSpecialTanks | GameOption::WithBoxes | GameOption::WithMines),"Unexpected Enemies - Reloaded"));
}

void FeatureMenu::accept()
{
   OptionMenu::accept();
   emit activate(new RoundsMenu());
}

void FeatureMenu::reject()
{
   emit activate(new InfoMenu(QImage(":images/splash.png")));
}
