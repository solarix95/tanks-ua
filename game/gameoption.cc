#include "gameoption.h"


GameOption::GameOption(QString desc)
{
    mOption = NoOption;
    mDescription = desc;
}

GameOption::GameOption(QSize mapSize, QString desc)
{
    mOption      = OpMapSize;
    mMapSize     = mapSize;
    mDescription = desc;
}

GameOption::GameOption(GameOption::Player player, QString name)
{
    mOption      = OpPlayerSelection;
    mPlayer      = player;
    mDescription = name;
}

GameOption::GameOption(int rounds, QString desc)
{
    mOption      = OpRounds;
    mRounds      = rounds;
    mDescription = desc;
}

GameOption::GameOption(Features features, QString desc)
{
    mOption      = OpFeatures;
    mFeatures    = features;
    mDescription = desc;
}

