#ifndef FEATUREMENU_H
#define FEATUREMENU_H

#include "optionmenu.h"

class FeatureMenu : public OptionMenu
{
public:
    FeatureMenu(Menu *next = NULL);

protected:
    virtual void accept();
    virtual void reject();
};

#endif // FEATUREMENU_H
