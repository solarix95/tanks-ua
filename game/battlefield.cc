#include "battlefield.h"
#include <QApplication>
#include <QPainter>
#include <QLinearGradient>
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include "global.h"
#include "infomenu.h"
#include "playermenu.h"

// --------------------------------------------------------------------------------
BattleField::BattleField(QWidget *parent) :
    QWidget(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    mGame = NULL;
    mMenu = NULL;

    setupDemoGame();
    registerGame(new Game(&mCamera,mDemoGameOptions));

    // Graphical update:
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(10);

    mFps = mFpsCounter = 0;
    connect(&mFpsTimer, SIGNAL(timeout()), this, SLOT(fps()));
    mFpsTimer.start(1000);
    initMenu();

    setWindowTitle("Tanks - Unexpected Enemies");
    resize(900,500);
}

// --------------------------------------------------------------------------------
void BattleField::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_F11) {
        if (!isFullScreen())
            setWindowState(Qt::WindowFullScreen);
        else
            showNormal();
    }

    if (mMenu)
        mMenu->keyPressed(ke->key());
    else {
        mGame->keyPress(ke->key());
    }
}

// --------------------------------------------------------------------------------
void BattleField::resizeEvent(QResizeEvent */*re*/)
{
    mWinSize = QSize(TUE_FIELD_HEIGHT*width()/height(),TUE_FIELD_HEIGHT);
    mCamera.setWinSize(mWinSize);
}

// --------------------------------------------------------------------------------
void BattleField::paintEvent(QPaintEvent *)
{
    mFpsCounter++;
    QPainter p(this);
    p.save();
    p.setWindow(0,0,mWinSize.width(),mWinSize.height());
    p.setRenderHint(QPainter::HighQualityAntialiasing);

    mGame->tdPaint(p,mWinSize);

    p.save();
    mCamera.setNewX();
    p.translate(-mCamera.x(),mWinSize.height());
    p.scale(1,-1);

    QRect region(mCamera.x(),0,mWinSize.width(),mWinSize.height());
    mGame->buPaint(p,region);

    p.restore();
    mGame->tdPaintHud(p,mWinSize, mFps);
    p.restore();
    if (mMenu)
        mMenu->render(p,rect());

    /*
    p.setPen(QPen(Qt::white));
    p.drawText(10,10,QString("%1fps").arg(mFps));
    */
}

// --------------------------------------------------------------------------------
void BattleField::fps()
{
    mFps = mFpsCounter;
    mFpsCounter= 0;
}

// --------------------------------------------------------------------------------
void BattleField::initMenu()
{
    registerMenu(new InfoMenu(QImage(":images/splash.png"), new InfoMenu(QImage(":images/help.png"))));
}

// --------------------------------------------------------------------------------
void BattleField::startGame()
{
    registerGame(new Game(&mCamera,mNextGameOptions));
    mNextGameOptions.clear();
    mMenu->deleteLater();
    mMenu = NULL;
}

// --------------------------------------------------------------------------------
void BattleField::endGame()
{
    Q_ASSERT(mGame);
    mGame->deleteLater();
    mGame = new Game(&mCamera,mDemoGameOptions);

    if (!mMenu)
        initMenu();
}

// --------------------------------------------------------------------------------
void BattleField::setupDemoGame()
{
    Q_ASSERT(mDemoGameOptions.isEmpty());
    mDemoGameOptions << GameOption(GameOption::Anarki,"Anarki");
    mDemoGameOptions << GameOption(GameOption::Anarki,"Anarki");
    mDemoGameOptions << GameOption(GameOption::Anarki,"Anarki");
    mDemoGameOptions << GameOption(GameOption::Iceman,"Iceman");
}

// --------------------------------------------------------------------------------
void BattleField::registerMenu(Menu *m)
{
    if (mMenu) {
        mMenu->deleteLater();
        mMenu = NULL;
    }
    mMenu = m;
    connect(mMenu,SIGNAL(activate(Menu*)),this, SLOT(registerMenu(Menu*)));
    connect(mMenu,SIGNAL(selectedOption(GameOption)), this, SLOT(registerOption(GameOption)));
    connect(mMenu, SIGNAL(startGame()),this, SLOT(startGame()));
}

// --------------------------------------------------------------------------------
void BattleField::registerOption(GameOption opt)
{
    mNextGameOptions << opt;
}

// --------------------------------------------------------------------------------

void BattleField::registerGame(Game *game)
{
    Q_ASSERT(game);
    if (mGame)
        mGame->deleteLater();
    mGame = game;
    connect(mGame,SIGNAL(gameEnded()), this, SLOT(endGame()));
}

