#include "infomenu.h"
#include "featuremenu.h"
#include <QImage>
#include <QCoreApplication>

// --------------------------------------------------------------------------------
InfoMenu::InfoMenu(const QImage &splash, Menu *nextMenu)
    : Menu(nextMenu)
{
    mSplash = splash;
}

// --------------------------------------------------------------------------------
void InfoMenu::keyPressed(int key)
{
    switch (key) {
      case Qt::Key_Space:
        accept();
        break;
     default:
        Menu::keyPressed(key);
    }
}

// --------------------------------------------------------------------------------
QSize InfoMenu::sizeHint() const
{
    return mSplash.size();
}

// --------------------------------------------------------------------------------
void InfoMenu::renderContent(QPainter &p)
{
    p.drawImage(0,0,mSplash);
}

// --------------------------------------------------------------------------------
void InfoMenu::accept()
{
    emit activate(new FeatureMenu());
}

// --------------------------------------------------------------------------------

void InfoMenu::reject()
{
    qApp->quit();
}


