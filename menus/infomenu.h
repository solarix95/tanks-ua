#ifndef INFOMENU_H
#define INFOMENU_H

#include <QImage>
#include "menu.h"

class InfoMenu : public Menu
{
public:
    InfoMenu(const QImage &splash,Menu *next = NULL);

protected:
    virtual void keyPressed(int key);
    virtual QSize sizeHint() const;
    virtual void renderContent(QPainter &p);
    virtual void accept();
    virtual void reject();

private:
    QImage mSplash;
};

#endif // INFOMENU_H
