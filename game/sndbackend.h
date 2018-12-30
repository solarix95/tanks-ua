#ifndef SNDBACKEND_H
#define SNDBACKEND_H

#include <QObject>

#ifdef WITH_SDL1
#include "sdlaudioplayer.h"
#else
#include "qtaudioplayer.h"
#endif

class SndBackend : public QObject
{
    Q_OBJECT
public:
    static SndBackend *instance();

    typedef enum eSound {
        LightShoot,
        LightBoom,
        MediumBoom,
        MetallicHit,
        WoodHit,
        ElectricZap,
        PlayerPoints,
        Thunder,
        Death,
        Youpi,
        GameInit
    } Sound;

    virtual void play(Sound s);
    
private:
    SndBackend();
    
    static SndBackend *mInstance;

#ifdef WITH_SDL1
    SdlAudioPlayer mPlayer;
#else
    QtAudioPlayer  mPlayer;
#endif
};

#endif // SNDBACKEND_H
