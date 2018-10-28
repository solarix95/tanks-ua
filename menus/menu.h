#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QRect>
#include <QSize>
#include <QPainter>
#include "gameoption.h"

class Menu : public QObject
{
    Q_OBJECT
public:
    Menu(Menu *nextMenu = NULL);

    void render(QPainter &p, QRect winSize);
    virtual void keyPressed(int key);

signals:
    void activate(Menu *newMenu);
    void selectedOption(GameOption opt);
    void startGame();

protected:
    inline Menu* nextMenu() const { return mNextMenu; }
    virtual QSize sizeHint() const = 0;

    // Rendering
    virtual void renderWindow(QPainter &p, QRect winSize);
    virtual void renderContent(QPainter &p) = 0;

    // Navigation
    virtual void accept();
    virtual void reject();
    virtual void nextItem();
    virtual void prevItem();


private:
    Menu *mNextMenu;
    
};

#endif // MENU_H
