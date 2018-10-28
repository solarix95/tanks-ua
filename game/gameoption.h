#ifndef GAMEOPTION_H
#define GAMEOPTION_H

#include <QObject>
#include <QSize>
#include <QList>

class GameOption
{
public:
    enum Option {
        NoOption,
        OpMapSize,
        OpPlayerSelection,
        OpRounds,
        OpFeatures
    } ;

    enum Player {
        LocalPlayer,
        Anarki,
        AngryDuke,
        Iceman
    } ;

    enum Feature {
        TanksOnly        = 0x00,
        WithEnvironment  = 0x01,  // Cloud, Trees
        WithSpecialTanks = 0x02,  // Swisstank
        WithMines        = 0x04,
        WithBoxes        = 0x08   // (Ammo)Boxes
    };
    Q_DECLARE_FLAGS(Features, Feature)

    GameOption(QString desc);
    GameOption(QSize mapSize, QString desc);
    GameOption(Player player, QString name);
    GameOption(int rounds, QString desc);
    GameOption(Features features, QString desc);

    inline Option   option() const     { return mOption;      }
    inline QSize    mapSize() const    { return mMapSize;     }
    inline int      rounds() const     { return mRounds;      }
    inline Player   player() const     { return mPlayer;      }
    inline QString  description()const { return mDescription; }
    inline Features features() const   { return mFeatures;    }
    
private:
    Option  mOption;
    QSize   mMapSize;
    int     mRounds;
    Player  mPlayer;
    QString mDescription;
    Features mFeatures;
};

typedef QList<GameOption>  GameOptions;

#endif // GAMEOPTION_H
