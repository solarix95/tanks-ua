#include <QMap>
#include <QFile>
#include <QDebug>
#include "sndbackend.h"

SndBackend *SndBackend::mInstance = NULL;

#define TEST_SDL_CHUNK(x)  if (!sChunks[x]) qWarning() << "Mix_QuickLoad_WAV" << Mix_GetError();

#ifdef WITH_SDL1
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
static QMap<int,Mix_Chunk*>  sChunks;
#endif

// --------------------------------------------------------------------------------
void* memdup(const void* mem, size_t size) {
   void* out = malloc(size);

   if(out != NULL)
       memcpy(out, mem, size);

   return out;
}

// --------------------------------------------------------------------------------
#ifdef WITH_SDL1
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
#endif

// --------------------------------------------------------------------------------
void SndBackend::play(SndBackend::Sound s)
{
#ifdef WITH_SDL1
    if (sChunks[s])
        Mix_PlayChannel(-1, sChunks[s], 0);
    else
        qWarning() << "SndBackend: cannot play" << s;
#endif
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

#ifdef WITH_SDL1
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
    Q_ASSERT(sChunks.isEmpty());
    sChunks[LightShoot]   = loadChunk(":sounds/artillery-shot.wav");  TEST_SDL_CHUNK(LightShoot); // Mix_LoadWAV("sounds/artillery-shot.wav");
    sChunks[LightBoom]    = loadChunk(":sounds/boom1.wav");           TEST_SDL_CHUNK(LightBoom);
    sChunks[MediumBoom]   = loadChunk(":sounds/boom2.wav");           TEST_SDL_CHUNK(MediumBoom);
    sChunks[MetallicHit]  = loadChunk(":sounds/steel-hammer.wav");    TEST_SDL_CHUNK(MetallicHit);
    sChunks[WoodHit]      = loadChunk(":sounds/wood-block-hit.wav");  TEST_SDL_CHUNK(WoodHit);
    sChunks[ElectricZap]  = loadChunk(":sounds/electric_zap.wav");    TEST_SDL_CHUNK(ElectricZap);
    sChunks[Thunder]      = loadChunk(":sounds/thunder.wav");         TEST_SDL_CHUNK(Thunder);
    sChunks[Death]        = loadChunk(":sounds/WilhelmScream.wav");   TEST_SDL_CHUNK(Death);
    sChunks[PlayerPoints] = loadChunk(":sounds/cash-register.wav");   TEST_SDL_CHUNK(PlayerPoints);
    sChunks[Youpi]        = loadChunk(":sounds/youpi.wav");           TEST_SDL_CHUNK(Youpi);
    sChunks[GameInit]     = loadChunk(":sounds/drums.wav");           TEST_SDL_CHUNK(GameInit);
#endif
}
