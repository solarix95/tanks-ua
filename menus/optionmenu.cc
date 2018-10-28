#include "optionmenu.h"

// --------------------------------------------------------------------------------
#define ICON_SIZE  35
#define ROW_SPACE  10
#define COL_SPACE  25

QMap<QString,int>  OptionMenu::sLastSelections;

// --------------------------------------------------------------------------------
OptionMenu::OptionMenu(const QString &ident,bool withNoOption, Menu *next) :
    Menu(next)
{
    if (withNoOption)
        mOptions << Option(QImage(":images/stopoption.png"),GameOption("end"));

    mIdent = ident;
    mSelection = sLastSelections[mIdent];
}

// --------------------------------------------------------------------------------
void OptionMenu::addOption(QImage icon, GameOption option)
{
    mOptions << Option(icon.scaled(QSize(ICON_SIZE,ICON_SIZE)),option);
}

// --------------------------------------------------------------------------------
QSize OptionMenu::sizeHint() const
{
    QSize s;
    s.setHeight(mOptions.count() * ICON_SIZE + (mOptions.count()-1)*ROW_SPACE+1);
    s.setWidth(ICON_SIZE + COL_SPACE + textBox().width()+3);
    return s;
}

// --------------------------------------------------------------------------------
void OptionMenu::renderContent(QPainter &p)
{
    p.save();
    QSize textSize = textBox();
    for (int i=0; i<mOptions.count(); i++) {
        p.drawImage(0,0,mOptions[i].icon);
        p.setPen(Qt::white);
        p.drawText(QRect(ICON_SIZE+COL_SPACE,0,textSize.width(),ICON_SIZE),Qt::AlignVCenter|Qt::AlignLeft,mOptions[i].option.description());
        if (i == mSelection) {
            p.setBrush(QBrush());
            p.setPen(Qt::red);
            p.drawRect(QRect(0,0,ICON_SIZE+COL_SPACE+textSize.width()+2,ICON_SIZE));
        }
        p.translate(0,+ICON_SIZE+ROW_SPACE);
    }
    p.restore();
}

// --------------------------------------------------------------------------------
void OptionMenu::accept()
{
    Q_ASSERT(mSelection >= 0 && mSelection < mOptions.count());
    if (mOptions[mSelection].option.option() != GameOption::NoOption) {
        sLastSelections[mIdent] = mSelection;
        emit selectedOption(mOptions[mSelection].option);
    }
}

// --------------------------------------------------------------------------------
void OptionMenu::nextItem()
{
    mSelection++;
    if (mSelection >= mOptions.count())
        mSelection = 0;
}

// --------------------------------------------------------------------------------
void OptionMenu::prevItem()
{
    mSelection--;
    if (mSelection < 0)
        mSelection = mOptions.count()-1;
}

// --------------------------------------------------------------------------------
GameOption OptionMenu::currentOption() const
{
    Q_ASSERT(mSelection >= 0 && mSelection < mOptions.count());
    return mOptions[mSelection].option;
}

// --------------------------------------------------------------------------------

QSize OptionMenu::textBox() const
{
    QString longestText;
    for (int i=0; i<mOptions.count(); i++)
        longestText = mOptions[i].option.description().length() > longestText.length() ?
                      mOptions[i].option.description() : longestText;
    QSize s;
    s.setWidth(QFontMetrics(QFont()).width(longestText));
    s.setHeight(QFontMetrics(QFont()).height());
    return s;
}
