#include <QFile>
#include <QDebug>
#include "game/qtaudioplayer.h"

#ifdef WITH_MULTIMEDIA
#include <QAudioOutput>
#include <QAudioFormat>
#include <QBuffer>

class AudioOutput {
public:
    AudioOutput(const QString &source)
        : output(NULL), buffer(&data) {
        QFile sourceFile;

        sourceFile.setFileName(source);
        sourceFile.open(QIODevice::ReadOnly);
        data = sourceFile.readAll();
        buffer.open(QIODevice::ReadOnly);

        QAudioFormat format;
        // Set up the format, eg.
        format.setSampleRate(44100);
        format.setChannelCount(2);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        if (!info.isFormatSupported(format)) {

            qWarning() << "AudioOutput: Format not supported" << format << "Fallback to" << info.preferredFormat();
            format = info.preferredFormat();
            // return;
        }

        output = new QAudioOutput(format);
        output->setBufferSize(qMax(data.count(),output->periodSize()));
    }
    ~AudioOutput() {
        if(output) delete output;
    }

    QAudioOutput  *output;
    QByteArray     data;
    QBuffer        buffer;
};

#endif

// --------------------------------------------------------------------------------
QtAudioPlayer::QtAudioPlayer()
{
}

// --------------------------------------------------------------------------------
void QtAudioPlayer::init(int sampleId, const QString &source)
{
#ifdef WITH_MULTIMEDIA
    mChunks[sampleId] = QList<AudioOutput*>() << new AudioOutput(source) << new AudioOutput(source);

    if (!mChunks[sampleId].first()->output)
        qDeleteAll(mChunks.take(sampleId));
#else
    Q_UNUSED(sampleId)
    Q_UNUSED(source)
#endif
}

// --------------------------------------------------------------------------------
void QtAudioPlayer::play(int sampleId)
{
#ifdef WITH_MULTIMEDIA
    if (!mChunks.contains(sampleId))
        return;

    for (int i=0; i<mChunks[sampleId].count(); i++) {
        if (mChunks[sampleId][i]->output->state() == QAudio::ActiveState)
            continue;
        mChunks[sampleId][i]->output->stop();  // Don't know why.. preventing audio "plop"?
        mChunks[sampleId][i]->buffer.seek(44); // skip wav header
        mChunks[sampleId][i]->output->start(&mChunks[sampleId][i]->buffer);
        return;
    }
#else
    Q_UNUSED(sampleId)
#endif
}

