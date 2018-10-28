#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QList>
#include <QMap>
#include <QString>
#include "menu.h"
#include "gameoption.h"

class OptionMenu : public Menu
{
    Q_OBJECT
public:
    explicit OptionMenu(const QString &ident,bool withNoOption = NULL, Menu *next = NULL);
    
    void addOption(QImage icon,GameOption option);

protected:
    virtual QSize sizeHint() const;
    virtual void renderContent(QPainter &p);
    virtual void accept();
    virtual void nextItem();
    virtual void prevItem();

    GameOption currentOption() const;

private:
    QSize textBox() const;

    typedef struct Option_ {
        QImage     icon;
        GameOption option;
        Option_(QImage img, GameOption opt) : icon(img), option(opt) {}
    } Option;

    QList<Option>  mOptions;
    int            mSelection;
    QString        mIdent;

    static QMap<QString,int>  sLastSelections;
};

#endif // OPTIONMENU_H
