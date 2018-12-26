#include "featuremenu.h"
#include "gameoption.h"
#include "roundsmenu.h"
#include "infomenu.h"

// --------------------------------------------------------------------------------
FeatureMenu::FeatureMenu(Menu *next)
    : OptionMenu("features",false,next)
{
    addOption(QImage(":images/start.png"),GameOption("Quick Match",GameOption::OpQuickMatch));
    addOption(QImage(":images/feature1.png"),GameOption(GameOption::Features(GameOption::TanksOnly),"Classic Tanks"));
    addOption(QImage(":images/feature2.png"),GameOption(GameOption::Features(GameOption::WithEnvironment),"Unexpected Enemies"));
    addOption(QImage(":images/feature3.png"),GameOption(GameOption::Features(GameOption::WithEnvironment | GameOption::WithSpecialTanks | GameOption::WithBoxes | GameOption::WithMines),"Unexpected Enemies - Reloaded"));
}

// --------------------------------------------------------------------------------
void FeatureMenu::accept()
{
    if (currentOption().option() == GameOption::OpQuickMatch) {
        emit selectedOption(GameOption(3,""));
        emit selectedOption(GameOption(GameOption::Features(GameOption::WithEnvironment | GameOption::WithSpecialTanks | GameOption::WithBoxes | GameOption::WithMines),""));
        emit selectedOption(GameOption(GameOption::LocalPlayer,"you"));
        emit selectedOption(GameOption(GameOption::Anarki,"Anarki"));
        emit selectedOption(GameOption(GameOption::AngryDuke,"Nerd"));
        emit selectedOption(GameOption(GameOption::Iceman,"Iceman"));
        emit startGame();
    } else {
        OptionMenu::accept();
        emit activate(new RoundsMenu());
    }
}

// --------------------------------------------------------------------------------
void FeatureMenu::reject()
{
    emit activate(new InfoMenu(QImage(":images/splash.png")));
}
