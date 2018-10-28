#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QMap>
#include "entity.h"
#include "tank.h"
#include "cloud.h"
#include "ammobox.h"
#include "sky.h"
#include "landscape.h"
#include "wind.h"
#include "player.h"
#include "shoot.h"
#include "explosion.h"
#include "camera.h"
#include "gameoption.h"

class Game : public QObject
{
    Q_OBJECT
public:

    enum State {
        StInit,
        StStartHint,
        StWaitForAction,
        StShooting,
        StSelectPlayer,
        StRoundEnd,
        StShowStatistics,
        StGameEnd
    };

    explicit Game(Camera *c, const GameOptions &options);
    virtual ~Game();
    
    void tdPaint(QPainter &p, QSize winSize);
    void buPaint(QPainter &p, QRect region);
    void tdPaintHud(QPainter &p, QSize size, int fpsInfo);
    void keyPress(int key);

    QSize size() const;

signals:
    void gameEnded();

public slots:
    Entity* registerEntity(Entity *e, bool prepend = false);
    Entity* registerTank(Tank *e);
    void    registerShoot(Shoot *e);
    void    registerExplosion(Explosion *e);

private slots:
    void unregisterEntity(QObject *e);
    void killEntity(Entity *e);

    void calculate();
    void gameState();
    void testCollision(QPointF p, bool *withGround);
    void testCollision(Entity *, Entity **withEntity);
    void updateGameLeader();

private:
    void initGame();
    void initRound();
    void initClouds();
    void initBombs();
    void initAmmoBoxes();
    void initTrees();
    void initTanks();
    void initPlayer();
    void selectRandomPlayer();
    void updatePlayerTargets();
    void updatePlayerAmmoBoxes();
    void placeEntity(Entity *e, int x = -1);
    int  playableTanks() const;
    bool hasFeature(GameOption::Feature f) const;

    void drawPlayerState(QPainter &p, const QSize &screenSize);
    void drawPlayerRanking(QPainter &p, const QSize &screenSize);
    void drawRadar(QPainter &p, const QSize &screenSize);
    void drawHelp(QPainter &p, const QSize &screenSize);
    void drawGameInfo(QPainter &p, const QSize &screenSize, int fpsInfo);
    void drawMenu(QPainter &p, QRect region, const QString &title = "", QColor textColor = Qt::white);
    void debugPrintGameState(QPainter &p, QSize screenSize);

    State             mState;
    QMap<int,QString> mStateNames;
    QTimer            mCalcTimer;
    QTime             mCalcTime;
    QTimer            mGameStateTimer;

    QList<Entity*>    mEntities; // Clouds, Tanks, Shoots, Explosions
    QList<Tank*>      mTanks;
    QList<Cloud*>     mClouds;
    QList<Player*>    mPlayer;
    QList<AmmoBox*>   mAmmoBoxes;
    int               mCurrentPlayer;
    QList<Shoot*>     mCurrentShoots;

    QSize             mMapSize;
    Sky              *mSky;
    Landscape        *mLandscape;

    Camera           *mCamera;
    Wind              mWind;

    QList<GameOption> mOptions;
    int               mRounds;
    int               mRound;
    bool              mShowStateBar;
    bool              mShowHelp;
    int               mGameInterval;
};

#endif // GAME_H
