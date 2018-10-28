#include <QDebug>
#include "camera.h"

#define INTERVAL 20

// --------------------------------------------------------------------------------
Camera::Camera(QObject *parent) :
    QObject(parent)
{
    mRequestedX = mCurrentX = 0;
    mLocked     = false;
    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(setNewX()));
    // mUpdateTimer.start(INTERVAL);
}

// --------------------------------------------------------------------------------
void Camera::setWinSize(QSize size)
{
    mWinSize = size;
    recalculate();
}

// --------------------------------------------------------------------------------
void Camera::setGamSize(QSize size)
{
    mGamSize = size;
    recalculate();
}

// --------------------------------------------------------------------------------
void Camera::requestCenter(int newX,int behaviour)
{
    int leftX = newX - mWinSize.width()/2;
    switch(behaviour) {
    case Hard:
        // avoid small jumps:
        if (!mLocked && (abs(mRequestedX-mCurrentX) > (mWinSize.width()/2))) {
            mCurrentX = leftX;
            break;
        }
    case Soft:
        if (!mLocked)
            mRequestedX = leftX;
        break;
    case StartEvent :
        if (mLocked)
            return;
        requestCenter(newX,Hard);
        mLocked = true;
        break;
    case EndEvent :
        mLocked = false;
        break;
    }
    recalculate();
}

// --------------------------------------------------------------------------------
void Camera::setNewX()
{
    float corr = 1; //mUpdateTime.elapsed()/(float)INTERVAL;
    // qDebug() << "X" << corr << mRequestedX << mCurrentX << (corr*((mRequestedX-mCurrentX)/150.0));
    mCurrentX += corr*((mRequestedX-mCurrentX)/120.0); // Smooth camera movement...
    mUpdateTime.start();
}

// --------------------------------------------------------------------------------
void Camera::recalculate()
{
    int maxX  = mGamSize.width()-mWinSize.width();

    if (mRequestedX < 0)
        mRequestedX = 0;
    if (mRequestedX > maxX)
        mRequestedX = maxX;


    if (mCurrentX < 0)
        mCurrentX = 0;
    if (mCurrentX > maxX)
        mCurrentX = maxX;

}
