#ifndef CAMERA_H
#define CAMERA_H

#include <QTimer>
#include <QTime>
#include <QSize>
#include <QObject>

class Camera : public QObject
{
    Q_OBJECT
public:
    typedef enum eBehaviour {
        Soft,        // Fly  to target
        Hard,        // Jump to Target
        StartEvent,  // Jump to Target and stay until "EndEvent"
        EndEvent
    } Behaviour;

    explicit Camera(QObject *parent = 0);
    
    void setWinSize(QSize size);
    void setGamSize(QSize size);

    inline int x() const { return mCurrentX;        }
    inline int w() const { return mWinSize.width(); }

public slots:
    void requestCenter(int x, int behaviour);
    void setNewX();

private:
    void recalculate();

    QTimer mUpdateTimer;
    QTime  mUpdateTime;
    float  mCurrentX;
    int    mRequestedX;
    QSize  mWinSize;
    QSize  mGamSize;
    bool   mLocked;  // StartEvent -> EndEvent
};

#endif // CAMERA_H
