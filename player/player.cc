#include <QDebug>
#include "player.h"
#include "sndbackend.h"

// --------------------------------------------------------------------------------
Player::Player(const QString &name, QObject *parent) :
    QObject(parent)
{
    mState       = StIdle;
    mTank        = NULL;
    mPoints      = 0;
    mKills       = 0;
    mName        = name;
    mIsLeader    = false;
}

// --------------------------------------------------------------------------------
void Player::setTank(Tank *t)
{
    mTank = t;
    Q_ASSERT(mTank);
    connect(mTank, SIGNAL(destroyed(QObject*)), this, SLOT(tankDestroyed(QObject*)));
    connect(mTank, SIGNAL(playerPoints(int)), this, SLOT(addPoints(int)));
    connect(mTank, SIGNAL(kills()), this, SLOT(incKills()));
    mTank->setIsLeader(mIsLeader);
}

// --------------------------------------------------------------------------------
void Player::setState(Player::State s)
{
    if (mState == s)
        return;
    mState = s;
    handleStateChange();
}

// --------------------------------------------------------------------------------
void Player::setIsLeader(bool isLeader)
{
    if (mTank)
        mTank->setIsLeader(isLeader);
    mIsLeader = isLeader;
}

// --------------------------------------------------------------------------------
void Player::setEnemyPositions(const QList<QPointF> enemies)
{
    mEnemies = enemies;
    handleNewEnemy();
}

// --------------------------------------------------------------------------------
void Player::setAmmoBoxes(const QList<QPointF> boxes)
{
    mAmmoBoxes = boxes;
    handleAmmoBoxes();
}

// --------------------------------------------------------------------------------
int Player::points() const
{
    return mPoints;
}

// --------------------------------------------------------------------------------
int Player::kills() const
{
    return mKills;
}

// --------------------------------------------------------------------------------
QString Player::name() const
{
    return mName;
}

// --------------------------------------------------------------------------------
bool Player::isLeader() const
{
    return mIsLeader;
}

bool Player::isLocalPlayer() const
{
    return false;
}

// --------------------------------------------------------------------------------
bool Player::handleKey(int)
{
    if (!mTank || mState != StActive)
        return false;
    return true;
}

// --------------------------------------------------------------------------------
void Player::handleStateChange()
{
    if (mTank)
        mTank->setSelected(mState != StIdle);
}

// --------------------------------------------------------------------------------
void Player::handleNewEnemy()
{
    // nur für Bots relevant
}

// --------------------------------------------------------------------------------
void Player::handleAmmoBoxes()
{
    // nur für Bots relevant
}

// --------------------------------------------------------------------------------
void Player::tankDestroyed(QObject *o)
{
    if (mTank == o)
        mTank = NULL;
}

// --------------------------------------------------------------------------------
void Player::addPoints(int pts)
{
    mPoints += pts;
    if (pts > 0)
        SndBackend::instance()->play(SndBackend::PlayerPoints);
    // qDebug() << this << name() << "ADD POINTS" << pts << mPoints;
    emit pointsChanged();
}

// --------------------------------------------------------------------------------
void Player::incKills()
{
    mKills++;
}
