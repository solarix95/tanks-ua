#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QWidget>
#include "game.h"
#include "camera.h"
#include "menu.h"
#include "gameoption.h"

class BattleField : public QWidget
{
    Q_OBJECT

public:
    explicit BattleField(QWidget *parent = 0);
    
protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent*);

signals:
    
public slots:
    void fps();

private slots:
    void initMenu();
    void startGame();
    void endGame();
    void registerMenu(Menu *m);
    void registerOption(GameOption opt);
    void registerGame(Game *game);

private:
    typedef enum MenuState_ {
        InitMenu,
        SelectPlayerMenu,
        NoMenu
    } MenuState;

    void    setupDemoGame();

    QSize    mWinSize;
    Game    *mGame;
    Menu    *mMenu;
    Camera   mCamera;

    QTimer   mFpsTimer;
    int      mFps;
    int      mFpsCounter;

    GameOptions mDemoGameOptions;
    GameOptions mNextGameOptions;
};

#endif // BATTLEFIELD_H
