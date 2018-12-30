#include <QMap>
#include <QFile>
#include <QDebug>
#include "sndbackend.h"

SndBackend *SndBackend::mInstance = NULL;

// --------------------------------------------------------------------------------
void SndBackend::play(SndBackend::Sound s)
{
    mPlayer.play(s);
}

// --------------------------------------------------------------------------------
SndBackend *SndBackend::instance()
{
    if (!mInstance)
        mInstance = new SndBackend();
    return mInstance;
}

// --------------------------------------------------------------------------------
SndBackend::SndBackend() :
    QObject()
{
    mPlayer.init(LightShoot,  ":sounds/artillery-shot.wav");
    mPlayer.init(LightBoom,   ":sounds/boom1.wav");
    mPlayer.init(MediumBoom,  ":sounds/boom2.wav");
    mPlayer.init(MetallicHit, ":sounds/steel-hammer.wav");
    mPlayer.init(WoodHit,     ":sounds/wood-block-hit.wav");
    mPlayer.init(ElectricZap, ":sounds/electric_zap.wav");
    mPlayer.init(Thunder,     ":sounds/thunder.wav");
    mPlayer.init(Death,       ":sounds/WilhelmScream.wav");
    mPlayer.init(PlayerPoints,":sounds/cash-register.wav");
    mPlayer.init(Youpi,       ":sounds/youpi.wav");
    mPlayer.init(GameInit,    ":sounds/drums.wav");
}
