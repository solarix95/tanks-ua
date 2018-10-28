#include <QTimer>
#include <QFontMetrics>
#include <QDebug>
#include "game.h"
#include "cloud.h"
#include "global.h"
#include "sndbackend.h"
#include "player.h"
#include "thinkerbot.h"
#include "vendettabot.h"
#include "localplayer.h"
#include "swisstank.h"
#include "tree.h"
#include "bigbomb.h"
#include "ammobox.h"
#include "ammobot.h"

#define STD_INTERVAL  50    // = 20Hz
#define MIN_DIST     100    // Minimum Distance between 2 entities

// --------------------------------------------------------------------------------
Game::Game(Camera *c, const GameOptions &options) :
    QObject(NULL)
{
    mCamera       = c;
    mState        = StInit;
    mCurrentPlayer= -1;
    mOptions      = options;
    mRounds       = 99;
    mRound        = 1;
    mShowStateBar = true;
    mShowHelp     = false;
    mGameInterval = STD_INTERVAL;

    mStateNames[StInit]          = "Init";
    mStateNames[StStartHint]     = "StartHint";
    mStateNames[StWaitForAction] = "WaitForAction";
    mStateNames[StShooting]      = "Shooting";
    mStateNames[StSelectPlayer]  = "SelectPlayer";
    mStateNames[StRoundEnd]      = "RoundEnd";
    mStateNames[StGameEnd]       = "GameEnd";
    mStateNames[StShowStatistics]= "ShowStatistics";

    mMapSize = QSize(TUE_FIELD_WIDTH,TUE_FIELD_HEIGHT);

    mSky          = new Sky(this);
    mLandscape    = new Landscape(this);
    connect(mLandscape, SIGNAL(entityCreated(Entity*,bool)), this, SLOT(registerEntity(Entity*,bool)));

    mCamera->setGamSize(mMapSize);

    connect(&mGameStateTimer, SIGNAL(timeout()), this, SLOT(gameState()));
    mGameStateTimer.start(100); // 10Hz für Spielezustände reichen

    mCalcTime.start();
    initGame();
}

// --------------------------------------------------------------------------------
Game::~Game()
{
    while(mEntities.count())
        mEntities.takeLast()->deleteLater();
    while(mPlayer.count())
        mPlayer.takeLast()->deleteLater();
}

// --------------------------------------------------------------------------------
void Game::tdPaint(QPainter &p, QSize winSize)
{
    calculate();
    mSky->tdPaint(p,winSize);
}

// --------------------------------------------------------------------------------
void Game::buPaint(QPainter &p, QRect region)
{
    mLandscape->buPaint(p,region);

    foreach(Entity *e, mEntities) {
        e->buPaint(p,region);
    }
}

// --------------------------------------------------------------------------------
void Game::tdPaintHud(QPainter &p, QSize size, int fpsInfo)
{
    switch(mState) {
    case StShowStatistics :
        drawPlayerRanking(p,size);
        break;
    case StStartHint :
    case StWaitForAction:
    case StShooting:
    case StSelectPlayer:
        if (mShowHelp)
            drawHelp(p,size);
        if (mShowStateBar) {
            drawPlayerState(p,size);
            drawRadar(p,size);
        }
        break;
    default: break;
    }

    drawGameInfo(p,size, fpsInfo);
    // debugPrintGameState(p,size);
}

// --------------------------------------------------------------------------------
void Game::keyPress(int key)
{
    if (mState == StShowStatistics) {
        mState = StGameEnd;
        emit gameEnded();
        return;
    }

    if (key == Qt::Key_Tab)
        mShowStateBar = !mShowStateBar;

    if (key == Qt::Key_F5)
        mGameInterval = mGameInterval == STD_INTERVAL ? STD_INTERVAL/3 : STD_INTERVAL;

    if (key == Qt::Key_F1)
        mShowHelp = !mShowHelp;

    if (key == Qt::Key_Escape && mState != StShowStatistics)
        mState = StShowStatistics;

    if (mCurrentPlayer < 0)
        return;

    Q_ASSERT(mCurrentPlayer < mPlayer.count());
    mPlayer[mCurrentPlayer]->handleKey(key);
}

// --------------------------------------------------------------------------------
QSize Game::size() const
{
    return mMapSize;
}

// --------------------------------------------------------------------------------
Entity* Game::registerEntity(Entity *e, bool prepend)
{
    Q_ASSERT(e);
    if (prepend)
        mEntities.prepend(e);
    else
        mEntities << e;
    connect(e, SIGNAL(testCollision(QPointF,bool*)), this, SLOT(testCollision(QPointF,bool*)));
    connect(e, SIGNAL(testCollision(Entity*,Entity**)), this, SLOT(testCollision(Entity*,Entity**)));
    connect(e, SIGNAL(setCamera(int,int)), mCamera, SLOT(requestCenter(int,int)));
    connect(e, SIGNAL(requestKill(Entity*)), this, SLOT(killEntity(Entity*)));
    connect(e, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterEntity(QObject*)));
    connect(e, SIGNAL(entityCreated(Entity*,bool)), this, SLOT(registerEntity(Entity*,bool)));
    connect(e, SIGNAL(explosion(Explosion*)), this, SLOT(registerExplosion(Explosion*)), Qt::QueuedConnection);
    return e;
}

// --------------------------------------------------------------------------------
Entity* Game::registerTank(Tank *e)
{
    Q_ASSERT(e);
    connect(e, SIGNAL(shoot(Shoot*)), this, SLOT(registerShoot(Shoot*)));
    mTanks << e;
    registerEntity(e);
    return e;
}

// --------------------------------------------------------------------------------
void Game::registerShoot(Shoot *s)
{
    mCurrentShoots << s;
    registerEntity(s);
    connect(s, SIGNAL(shoot(Shoot*)), this, SLOT(registerShoot(Shoot*)));
    mState = StShooting;
    Q_ASSERT(mCurrentPlayer >= 0 && mCurrentPlayer < mPlayer.count());
    mPlayer[mCurrentPlayer]->setState(Player::StPassive);
}

// --------------------------------------------------------------------------------
void Game::registerExplosion(Explosion *e)
{
    registerEntity(e);
    foreach(Entity *nextE, mEntities)
        nextE->handleExplosion(e);
    mLandscape->handleExplosion(e);

}

// --------------------------------------------------------------------------------
void Game::unregisterEntity(QObject *e)
{
    mEntities.removeAll((Entity*)e);
    mClouds.removeAll((Cloud*)e);
    if (mTanks.contains((Tank*)e)) {
        mTanks.removeAll((Tank*)e);
        updatePlayerTargets(); // select new Enemies..
    }
    if (mAmmoBoxes.contains((AmmoBox*)e)) {
        mAmmoBoxes.removeAll((AmmoBox*)e);
        updatePlayerAmmoBoxes();
    }
    if (mCurrentShoots.contains((Shoot*)e)) {
        mCurrentShoots.removeAll((Shoot*)e);
        if (mCurrentShoots.isEmpty()) {
            mState = StSelectPlayer;
        }
        return;
    }
}

// --------------------------------------------------------------------------------
void Game::killEntity(Entity *e)
{
    if (mClouds.count() > 0) {
        int killerIndex = qrand() % mClouds.count();
        Q_ASSERT(killerIndex >= 0 && killerIndex < mClouds.count());
        mClouds[killerIndex]->destroyTarget(e);
    }
}

// --------------------------------------------------------------------------------
void Game::calculate()
{
    float gameSpeed = mCalcTime.elapsed()/(float)mGameInterval;
    if (gameSpeed <= 0) { // 2x calculate in folge..
        return;
    }
    mWind.calculate(gameSpeed);
    foreach(Entity *e, mEntities) {
        e->calculate(gameSpeed,mWind.force());
    }
    mCalcTime.start();
}

// --------------------------------------------------------------------------------
void Game::gameState()
{
    switch(mState) {
    case StInit : {
        initRound();
    } break;
    case StStartHint : {
        foreach(Entity *e, mEntities) {
            if (e->state() != Entity::StIdle)
                return;
        }

        Q_ASSERT(mCurrentPlayer >= 0 && mCurrentPlayer < mPlayer.count());
        if (!mPlayer[mCurrentPlayer]->tank()) { // was wenn der Tank während dem State StartHint zerstört wird..
            mState = StSelectPlayer;
            return;
        }

        mState = StWaitForAction;
        mCamera->requestCenter(mPlayer[mCurrentPlayer]->tank()->pos().x(),Camera::Hard);
        mPlayer[mCurrentPlayer]->setState(Player::StActive);
        if (mPlayer[mCurrentPlayer]->isLocalPlayer())
            SndBackend::instance()->play(SndBackend::GameInit);
    } break;
    case StWaitForAction: {
    } break;
    case StSelectPlayer : {
        foreach(Entity *e, mEntities) {
            if (e->state() != Entity::StIdle) {
                return;
            }
        }
        Q_ASSERT(mCurrentPlayer >= 0 && mCurrentPlayer < mPlayer.count());
        mPlayer[mCurrentPlayer]->setState(Player::StIdle);
        mCurrentPlayer++;
        if (mCurrentPlayer >= mPlayer.count())
            mCurrentPlayer = 0;
        if (!mPlayer[mCurrentPlayer]->canShoot() && mTanks.count() > 1) // Select another Player..
            return;
        if (playableTanks() > 1)
            mState = StStartHint;
        else
            mState = StRoundEnd;
    } break;
    case StRoundEnd: {
        mRound++;
        if (mRound > mRounds) {
            mState = StShowStatistics;
        } else
            mState = StInit;
    } break;
    default: break;
    }
}

// --------------------------------------------------------------------------------
void Game::testCollision(QPointF p, bool *withGround)
{
    Q_ASSERT(withGround);
    Q_ASSERT(mLandscape);
    *withGround = mLandscape->collide(p);
}

// --------------------------------------------------------------------------------
void Game::testCollision(Entity *e, Entity **withEntity)
{
    Q_ASSERT(e);
    Q_ASSERT(withEntity);
    foreach(Entity *next, mEntities) {
        if (e == next)
            continue;
        if (e->boundingBox().intersects(next->boundingBox())) {
            *withEntity = next;
            return;
        }
    }
}

// --------------------------------------------------------------------------------
void Game::updateGameLeader()
{
    Player *oldLeader = NULL;
    Player *newLeader    = NULL;
    foreach(Player *p, mPlayer) {
        if (!newLeader || p->points() > newLeader->points()) // TODO: was wenn gleich viel?
            newLeader = p;
        if (p->isLeader())
            oldLeader = p;
    }
    if (newLeader && newLeader->points() <= 0) // Bei Spielstart gibts kein Leader..
        newLeader = NULL;

    foreach(Player *p, mPlayer) {
        p->setIsLeader(p == newLeader);
    }

    if (newLeader && (newLeader != oldLeader))
        SndBackend::instance()->play(SndBackend::Youpi);
}

// --------------------------------------------------------------------------------
void Game::initGame()
{
    Q_ASSERT(mLandscape);
    Q_ASSERT(mPlayer.isEmpty());

    for(int i=0; i<mOptions.count(); i++) {
        if (mOptions[i].option() == GameOption::OpRounds) {
            mRounds = mOptions[i].rounds();
        }
    }
    initPlayer();
}

// --------------------------------------------------------------------------------
void Game::initRound()
{
    // Cleanup
    while (mTanks.count())
        mTanks.takeLast()->deleteLater();
    while (mEntities.count())
        mEntities.takeLast()->deleteLater();

    mLandscape->init(mMapSize);

    // Player gut durchschütteln..
    QList<Player*> players = mPlayer;
    mPlayer.clear();
    Q_ASSERT(players.count() >= 2);
    mPlayer << players.takeFirst();
    while (!players.isEmpty()) {
        mPlayer.insert(qrand() % (mPlayer.count()+1),players.takeFirst());
    }

    initTanks();
    selectRandomPlayer();

    if (hasFeature(GameOption::WithEnvironment)) {
        initTrees();
        initClouds();
    }

    if (hasFeature(GameOption::WithBoxes)) {
        initAmmoBoxes();
    }

    if (hasFeature(GameOption::WithMines)) {
        initBombs();
    }

    updatePlayerTargets();
    updatePlayerAmmoBoxes();
    mState         = StStartHint;
}

// --------------------------------------------------------------------------------
void Game::initClouds()
{
    int minX = mMapSize.width()*0.1;
    for (int i=0; i< qrand() % 4; i++) {
        mClouds << new Cloud(QPoint(minX+qrand()%(int)(mMapSize.width()*0.8),mMapSize.height()*0.8));
        registerEntity(mClouds.last());
    }
}

// --------------------------------------------------------------------------------
void Game::initBombs()
{
    Q_ASSERT(mLandscape);
    for (int i=0; i< qrand() % 16; i++) {
        int bombX = mMapSize.width()*0.1 + qrand()%(int)(mMapSize.width()*0.8);
        int bombY = mLandscape->heightAt(bombX)/2 + qrand()%(mLandscape->heightAt(bombX)/2) - 10;
        registerEntity(new BigBomb(QPointF(bombX,bombY)));
    }
}

// --------------------------------------------------------------------------------
void Game::initAmmoBoxes()
{
    int mediumCount  = mPlayer.count() * 0.6;
    int scatterCount = qMax((int)(mPlayer.count() * 0.2),1);
    int hammerCount  = qMax((int)(mPlayer.count() * 0.2),1);
    int atomicCount  = qMax((int)(mPlayer.count() * 0.2),1);


    for (int i=0; i<mediumCount; i++) {
        AmmoBox *box = new AmmoBox(QPointF(0,mMapSize.height()*0.9), Bullet::MediumShoot);
        placeEntity(registerEntity(box));
        mAmmoBoxes << box;
    }
    for (int i=0; i<scatterCount; i++) {
        AmmoBox *box = new AmmoBox(QPointF(0,mMapSize.height()*0.9), Bullet::ScatterShoot);
        placeEntity(registerEntity(box));
        mAmmoBoxes << box;
    }
    for (int i=0; i<hammerCount; i++) {
        AmmoBox *box = new AmmoBox(QPointF(0,mMapSize.height()*0.9), Bullet::HammerShoot);
        placeEntity(registerEntity(box));
        mAmmoBoxes << box;
    }
    for (int i=0; i<atomicCount; i++) {
        AmmoBox *box = new AmmoBox(QPointF(0,mMapSize.height()*0.9), Bullet::AtomicShoot);
        placeEntity(registerEntity(box));
        mAmmoBoxes << box;
    }
}

// --------------------------------------------------------------------------------
void Game::initTrees()
{
    int treeCount = qrand() % mPlayer.count();
    for (int i=0; i< treeCount;i++) {
        placeEntity(registerEntity(new Tree(QPoint(0,mMapSize.height()*0.9))));
    }
}

// --------------------------------------------------------------------------------
void Game::initTanks()
{
    for (int i=0; i<mPlayer.count(); i++) {
        if (i == 0)
            registerTank(new Tank(QPointF(mMapSize.width()*0.2,mMapSize.height()*0.9)));
        else if (i == 1)
            registerTank(new Tank(QPointF(mMapSize.width()*0.8,mMapSize.height()*0.9)));
        else
            placeEntity(registerTank(new Tank(QPointF(0,mMapSize.height()*0.9))));
    }

    if (hasFeature(GameOption::WithSpecialTanks))
        placeEntity(registerTank(new SwissTank(QPointF(0,mMapSize.height()*0.9))));

    Q_ASSERT(mTanks.count() >= mPlayer.count());
    for(int i=0; i<mPlayer.count();i++)
        mPlayer[i]->setTank(mTanks[i]);
}

// --------------------------------------------------------------------------------
void Game::initPlayer()
{
    Q_ASSERT(mPlayer.isEmpty());

    for(int i=0; i<mOptions.count(); i++) {
        if (mOptions[i].option() == GameOption::OpPlayerSelection) {
            switch(mOptions[i].player()) {
            case GameOption::LocalPlayer:
                mPlayer << new LocalPlayer("YOU");
                break;
            case GameOption::AngryDuke:
                mPlayer << new VendettaBot(mOptions[i].description());
                break;
            case GameOption::Iceman:
                mPlayer << new AmmoBot(mOptions[i].description());
                break;
            default:
                mPlayer << new ThinkerBot(mOptions[i].description());
                break;
            }
            connect(mPlayer.last(), SIGNAL(pointsChanged()), this, SLOT(updateGameLeader()));
        }
    }
}

// --------------------------------------------------------------------------------
void Game::selectRandomPlayer()
{
    // spielbare Player zusammenstellen
    QList<Player*>    tankPlayer;
    foreach (Player *p, mPlayer) {
        if (p->tank())
            tankPlayer << p;
    }

    Q_ASSERT(!tankPlayer.isEmpty());

    int playerIndex = qrand() % tankPlayer.count();

    mCurrentPlayer = mPlayer.indexOf(tankPlayer[playerIndex]);
    Q_ASSERT(mCurrentPlayer >= 0);
}

// --------------------------------------------------------------------------------
void Game::updatePlayerTargets()
{
    foreach(Player *p, mPlayer) {
        QList<QPointF> enemyList;
        foreach(Tank *t, mTanks) {
            if (t != p->tank() && t->isPlayable())
                enemyList << t->pos();
        }
        p->setEnemyPositions(enemyList);
    }
}

// --------------------------------------------------------------------------------
void Game::updatePlayerAmmoBoxes()
{
    QList<QPointF> boxList;
    foreach(AmmoBox *b, mAmmoBoxes)
        boxList << b->pos();

    foreach(Player *p, mPlayer)
        p->setAmmoBoxes(boxList);
}

// --------------------------------------------------------------------------------
void Game::placeEntity(Entity *e, int x)
{
    static int overflow = 0;
    overflow++;

    Q_ASSERT(overflow < 500);
    if (x <= 0) {
        placeEntity(e,mMapSize.width()*0.1 + (qrand() % ((int)(mMapSize.width()*0.9))));
        overflow--;
        return;
    }
    foreach(Entity *o, mEntities) {
        if (e == o)
            continue;
        if (abs(x - o->pos().x()) < MIN_DIST) {
            // qDebug() << "PLACE FAILED" << e << o << x << o->pos();
            placeEntity(e,mMapSize.width()*0.1 + (qrand() % ((int)(mMapSize.width()*0.9))));
            overflow--;
            return;
        }
    }
    e->setPos(QPointF(x,e->pos().y()));
    overflow--;
}

// --------------------------------------------------------------------------------
int Game::playableTanks() const
{
    int ret = 0;
    foreach(Tank *t, mTanks)
        ret += t->isPlayable() ? 1 : 0;
    return ret;
}

// --------------------------------------------------------------------------------
bool Game::hasFeature(GameOption::Feature f) const
{
    foreach(GameOption o, mOptions) {
        if (o.option() == GameOption::OpFeatures)
            return o.features().testFlag(f);
    }
    return false;
}

// --------------------------------------------------------------------------------
void Game::drawPlayerState(QPainter &p, const QSize &screenSize)
{
    const int centerOff = 200;

    if (mCurrentPlayer < 0 || mState == StRoundEnd)
        return;

    Q_ASSERT(mCurrentPlayer < mPlayer.count());
    if (!mPlayer[mCurrentPlayer]->tank())
        return;

    p.save();
    QFont f = p.font();
    f.setPointSize(f.pointSize()+5);
    p.setFont(f);
    QRect rect = p.fontMetrics().boundingRect(QString("XXXXX"));
    int firstX     = (screenSize.width() - rect.width())/2 - centerOff;
    int firstY     = screenSize.height() - 4*rect.height();
    int boxHeight = 2*rect.height();
    int boxWidth  = rect.width() + 50;

    drawMenu(p,QRect(firstX,firstY,boxWidth,boxHeight),mPlayer[mCurrentPlayer]->name(),mPlayer[mCurrentPlayer]->tank()->color());
    p.setPen(Qt::white);
    QRect textRect(firstX,firstY+5,rect.width(),rect.height());
    p.drawText(textRect,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(mPlayer[mCurrentPlayer]->tank()->angle(),0,'f',1));
    textRect.setY(textRect.y()+rect.height());
    textRect.setHeight(rect.height());
    p.drawText(textRect,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(mPlayer[mCurrentPlayer]->tank()->force(),0,'f',1));
    int   maxHealthHeight = 2*rect.height();
    float healthFactor    = qMax(mPlayer[mCurrentPlayer]->tank()->health()/100.0,0.0);
    int   healthHeight    = maxHealthHeight * healthFactor;
    static QColor green   = QColor(Qt::green);
    p.setBrush(QColor(green.red()*healthFactor,green.green()*healthFactor, green.blue()*healthFactor));
    p.drawRect(QRect(firstX+rect.width()+10,firstY+maxHealthHeight-healthHeight,30,healthHeight));
    p.restore();
}

// --------------------------------------------------------------------------------
void Game::drawPlayerRanking(QPainter &p, const QSize &screenSize)
{
    const int rowDist   = 10;
    Q_ASSERT(mPlayer.count() >= 2);

    p.save();
    QFont f = p.font();
    f.setPointSize(f.pointSize()+5);
    p.setFont(f);
    QRect singleRect = p.fontMetrics().boundingRect(QString("XXXXX"));
    int neededHeight = singleRect.height() * mPlayer.count() + (mPlayer.count()-1) * rowDist;

    int firstY     = screenSize.height()/2 - neededHeight/2;
    if (firstY < 30)
        firstY = 30;
    int firstX = screenSize.width()/2 - singleRect.width()/2;
    p.setPen(Qt::white);
    QMap<int,Player*> sortedPlayer;
    foreach(Player *p, mPlayer)
        sortedPlayer.insert(p->points(),p);
    QMap<int,Player*>::const_iterator it = sortedPlayer.constEnd();
    p.translate(firstX,firstY);
    int rank = 1;
    while (it != sortedPlayer.begin()) {
        it--;
        p.drawText(0,0,QString("%1: %2 (%3pts, %4 kills)").arg(rank++).arg(it.value()->name()).arg(it.key()).arg(it.value()->kills()));
        p.translate(0,singleRect.height() + rowDist);
    };
    p.restore();
}

// --------------------------------------------------------------------------------
void Game::drawRadar(QPainter &p, const QSize &screenSize)
{
    const int centerOff = -200;

    if (mCurrentPlayer < 0 || mState == StRoundEnd)
        return;
    if (!mPlayer[mCurrentPlayer]->tank())
        return;

    p.save();
    QFont f = p.font();
    f.setPointSize(f.pointSize()+5);
    p.setFont(f);
    QRect rect = p.fontMetrics().boundingRect(QString("XXXXX"));
    int firstX     = (screenSize.width() - rect.width())/2 - centerOff;
    int firstY     = screenSize.height() - 4*rect.height();
    int boxHeight = 2*rect.height();
    int boxWidth  = rect.width() + 50;

    drawMenu(p,QRect(firstX,firstY,boxWidth,boxHeight),"Battlefield");
    p.translate(firstX,firstY+boxHeight);

    float scaleX = boxWidth/(float)mMapSize.width();
    float scaleY = boxHeight/(float)mMapSize.height();

    p.scale(scaleX,-scaleY);
    p.setClipRect(QRect(0,0,mMapSize.width(),mMapSize.height()));

    /* DEBUG
    p.setBrush(QBrush());
    p.setPen(Qt::red);
    p.drawRect(0,0,mMapSize.width()-5,mMapSize.height()-5);
    */

    p.setBrush(Qt::gray);
    p.setPen(QPen(Qt::green,2/scaleX));

    p.drawLine(mCamera->x(),0,mCamera->x(),mMapSize.height());
    p.drawLine(mCamera->x()+mCamera->w(),0,mCamera->x()+mCamera->w(),mMapSize.height());

    p.setPen(QPen());
    foreach(Player *pl, mPlayer) { // mPlayer besser als mTanks (Swisstank wird dadurch nicht gerendert)
        if (pl->tank()) {
            p.setBrush(pl->tank()->color());
            p.drawEllipse(pl->tank()->pos(),5/scaleX,5/scaleY);
        }
    }

    p.setBrush(AmmoBox::color());
    foreach(AmmoBox *box, mAmmoBoxes) {
        p.save();
        p.translate(box->pos());
        p.drawRect(QRect(-5/scaleX,5/scaleY,10/scaleX,10/scaleY));
        p.restore();
    }

    p.setBrush(Qt::white);
    foreach(Shoot *s, mCurrentShoots)
        p.drawEllipse(s->pos(),3/scaleX,3/scaleY);

    p.restore();
}

// --------------------------------------------------------------------------------
void Game::drawHelp(QPainter &p, const QSize &screenSize)
{
    static const QString helpText = "Arrows : Angle/Force\nSpace: Shoot\nTab: Toggle Statusbar\nF5: Toggle Speed";

    p.save();
    QFont f = p.font();
    f.setPointSize(f.pointSize()+5);
    p.setFont(f);
    p.setPen(Qt::white);
    int flags = Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap;
    QRect rect = p.fontMetrics().boundingRect(0,0,500,500,flags,helpText);

    int boxHeight = rect.height() + 30;
    int boxWidth  = rect.width()  + 30;

    int firstX     = screenSize.width()/2 - boxWidth/2;
    int firstY     = screenSize.height()/2 - boxHeight/2;

    p.translate(firstX,firstY);

    drawMenu(p,QRect(0,0,boxWidth,boxHeight),"Help");

    p.drawText(QRect(0,0,boxWidth,boxHeight),flags,helpText);

    p.restore();
}

// --------------------------------------------------------------------------------
void Game::drawGameInfo(QPainter &p, const QSize &screenSize, int fpsInfo)
{
    p.save();
    p.setPen(Qt::white);
    QFont f = p.font();
    f.setPointSize(14);
    p.setFont(f);
    p.drawText(20,20,QString("FPS: %1").arg(fpsInfo));
    if (mRound <= mRounds) { // Rundenzahl nur während Match anzeigen
        p.drawText(20,40,QString("Round: %1/%2").arg(mRound).arg(mRounds));
    }
    p.restore();
}

// --------------------------------------------------------------------------------
void Game::drawMenu(QPainter &p, QRect region, const QString &title, QColor textColor)
{
    static const int radius   = 10;
    static const int padding  = 20;

    QRect titleRect = p.fontMetrics().boundingRect(QString(title));

    p.save();
    p.setPen(Qt::white);
    p.setBrush(Qt::black);
    int boxX = region.x()-padding;
    int boxY = region.y()-padding;
    int boxW = region.width()+2*padding;
    int boxH = region.height()+2*padding;
    p.drawRoundedRect(QRect(boxX,boxY,boxW,boxH),radius,radius);

    if (!title.isEmpty()) {
        static const int radius   = 3;
        static const int padding  = 4;

        int subBoxW = titleRect.width()  + 2*padding;
        int subBoxH = titleRect.height() + 2*padding;
        int subBoxX = boxX + boxW/2 - subBoxW/2;
        int subBoxY = boxY - subBoxH/2 - 3;
        p.drawRoundedRect(QRect(subBoxX,subBoxY,subBoxW,subBoxH),radius,radius);
        p.setPen(textColor);
        p.drawText(QRect(subBoxX,subBoxY,subBoxW,subBoxH), Qt::AlignCenter,title);
    }
    p.restore();
}

// --------------------------------------------------------------------------------
void Game::debugPrintGameState(QPainter &p, QSize screenSize)
{
    p.setPen(Qt::gray);
    p.drawText(QRectF(0,0,screenSize.width(),screenSize.height()),mStateNames[mState],Qt::AlignHCenter | Qt::AlignVCenter);
}
