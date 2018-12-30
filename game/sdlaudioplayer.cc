#include <QFile>
#include <QByteArray>
#include <QDebug>
#include "game/sdlaudioplayer.h"

// --------------------------------------------------------------------------------
void* memdup(const void* mem, size_t size) {
   void* out = malloc(size);

   if(out != NULL)
       memcpy(out, mem, size);

   return out;
}

// --------------------------------------------------------------------------------
Mix_Chunk *loadChunk(const QString &resource) {
    QFile res(resource);
    res.open(QIODevice::ReadOnly);

    QByteArray sndData = res.readAll();

    // Funktioniert nicht, weil WAV-Format einer SDL-Vorgabe entsprechen müsste
    // Mix_Chunk *chunk = Mix_QuickLoad_WAV((quint8*)sndData.data());

    SDL_RWops *ops = SDL_RWFromConstMem(memdup(sndData.constData(),sndData.size()),sndData.size());


    Mix_Chunk *chunk = Mix_LoadWAV_RW(ops,1 /* 1 -> Speicher von memdup() wieder freigeben */);

    return chunk;
}

// --------------------------------------------------------------------------------
SdlAudioPlayer::SdlAudioPlayer()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100,AUDIO_S16SYS,2,640);
}

// --------------------------------------------------------------------------------
void SdlAudioPlayer::init(int sampleId, const QString &source)
{
    Q_ASSERT(!mChunks.contains(sampleId));

    mChunks[sampleId]   = loadChunk(source);
    if (!mChunks[sampleId])
        qWarning() << "SdlAudioPlayer::init: Mix_QuickLoad_WAV Error" << Mix_GetError();
}

// --------------------------------------------------------------------------------
void SdlAudioPlayer::play(int sampleId)
{
    if (mChunks[sampleId])
        Mix_PlayChannel(-1, mChunks[sampleId], 0);
    else
        qWarning() << "SdlAudioPlayer: cannot play" << sampleId;
}

