#ifndef SDLAUDIOPLAYER_H
#define SDLAUDIOPLAYER_H

#include <QString>
#include <QMap>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class SdlAudioPlayer
{
public:
    SdlAudioPlayer();

    void init(int sampleId, const QString &source);
    void play(int sampleId);

private:
    QMap<int,Mix_Chunk*>  mChunks;
};

#endif // SDLAUDIOPLAYER_H
