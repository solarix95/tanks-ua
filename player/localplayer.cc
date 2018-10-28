#include "localplayer.h"

//-----------------------------------------------------------------------------
LocalPlayer::LocalPlayer(const QString &name, QObject *parent) :
    Player(name,parent)
{
}

//-----------------------------------------------------------------------------
bool LocalPlayer::handleKey(int key)
{
    if (state() != Player::StActive || !tank())
        return false;

    switch(key) {
      case Qt::Key_Up: {
        tank()->setForce(tank()->force()+1);
      } break;
      case Qt::Key_Down: {
        if (tank()->force() >= 2)
            tank()->setForce(tank()->force()-1);
      } break;
      case Qt::Key_Left: {
       tank()->setAngle(tank()->angle()+1);
      } break;
      case Qt::Key_Right: {
       tank()->setAngle(tank()->angle()-1);
      } break;
      case Qt::Key_Space: {
       tank()->fire();
      } break;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool LocalPlayer::isLocalPlayer() const
{
    return true;
}
