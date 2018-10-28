#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "player.h"

class LocalPlayer : public Player
{
    Q_OBJECT
public:
    explicit LocalPlayer(const QString &name,QObject *parent = 0);

    virtual bool handleKey(int key);
    virtual bool isLocalPlayer() const;
    
signals:
    
public slots:
    
};

#endif // LOCALPLAYER_H
