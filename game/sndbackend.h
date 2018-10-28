#ifndef SNDBACKEND_H
#define SNDBACKEND_H

#include <QObject>

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
    
signals:
    
public slots:
private:
    SndBackend();
    
    static SndBackend *mInstance;
};

#endif // SNDBACKEND_H
