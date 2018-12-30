#ifndef QTAUDIOPLAYER_H
#define QTAUDIOPLAYER_H

#include <QString>
#include <QMap>
#include <QObject>
#include <QList>

class AudioOutput;
class QtAudioPlayer : public QObject
{
public:
    QtAudioPlayer();

    void init(int sampleId, const QString &source);
    void play(int sampleId);

private:
    QMap<int,QList<AudioOutput*> >  mChunks;
};

#endif // QTAUDIOPLAYER_H
