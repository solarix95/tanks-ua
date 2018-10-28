#include "menu.h"

#define PADDING 15

// --------------------------------------------------------------------------------
Menu::Menu(Menu *nextMenu) :
    QObject()
{
    mNextMenu = nextMenu;
}

// --------------------------------------------------------------------------------
void Menu::render(QPainter &p, QRect winSize)
{
    p.save();
    renderWindow(p,winSize);
    renderContent(p);
    p.restore();
}

// --------------------------------------------------------------------------------
void Menu::keyPressed(int key)
{
    switch (key) {
      case Qt::Key_Return:
      case Qt::Key_Enter:
      case Qt::Key_Space:
        accept();
        break;
      case Qt::Key_Escape:
        reject();
        break;
      case Qt::Key_Tab:
      case Qt::Key_Down:
        nextItem();
        break;
      case Qt::Key_Up:
        prevItem();
        break;
    }
}

// --------------------------------------------------------------------------------
void Menu::accept()
{
}

// --------------------------------------------------------------------------------
void Menu::reject()
{
}

// --------------------------------------------------------------------------------
void Menu::nextItem()
{
}

// --------------------------------------------------------------------------------
void Menu::prevItem()
{
}

// --------------------------------------------------------------------------------
void Menu::renderWindow(QPainter &p, QRect winSize)
{
    QSize mySize = sizeHint();
    int sx = winSize.width()/2  - mySize.width()/2  - PADDING;
    int sy = winSize.height()/2 - mySize.height()/2 - PADDING;
    p.setBrush(Qt::black);
    p.setPen(Qt::green);
    p.drawRoundedRect(sx,sy,mySize.width()+2*PADDING,mySize.height()+2*PADDING,PADDING,PADDING);
    p.translate(sx+PADDING,sy+PADDING);
    p.setClipRect(0,0,mySize.width(),mySize.height());
}

