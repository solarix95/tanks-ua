#include <QApplication>

#include "battlefield.h"

// --------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    BattleField f;
    f.show();

    return app.exec();
}
