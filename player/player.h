#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "tank.h"

class Player : public QObject
{
    Q_OBJECT

public:
    typedef enum eState {
        StIdle,      // ein anderer Player ist an der Reihe
        StActive,    // darf schiessen
        StPassive    // wartet auf Ende des Schusses
    } State;

    explicit Player(const QString &name, QObject *parent = 0);
    
    virtual void setTank(Tank *t);
    virtual void setState(State s);
    virtual void setIsLeader(bool isLeader);

    /*
      Infos für Bots: wo befinden sich Gegner + Munition
    */
    void    setEnemyPositions(const QList<QPointF> enemies);
    void    setAmmoBoxes(const QList<QPointF> boxes);

    // Player Metadaten:
    int     points() const;
    int     kills() const;
    QString name() const;
    bool    isLeader() const;
    virtual bool isLocalPlayer() const;


    // Keyboard-Input:
    virtual bool handleKey(int key);

    inline Tank*  tank() const           {  return mTank;  }
    inline bool canShoot() const         { return mTank && mTank->state() == Entity::StIdle; }

signals:
    void pointsChanged();

protected:
    inline State   state() const           { return mState;             }
    inline int     enemyCount() const      { return mEnemies.count();   }
    inline QPointF enemyPos(int i) const   { return mEnemies[i];        }
    inline int     ammoBoxCount() const    { return mAmmoBoxes.count(); }
    inline QPointF ammoBoxPos(int i) const { return mAmmoBoxes[i];      }

    virtual void handleStateChange();
    virtual void handleNewEnemy();     // Bots: Auswahl der Targets
    virtual void handleAmmoBoxes();    // Bots: Auswahl der Targets

private slots:
    void tankDestroyed(QObject*);
    void addPoints(int pts);
    void incKills();

private:
    State           mState;
    Tank           *mTank;
    QList<QPointF>  mEnemies;
    QList<QPointF>  mAmmoBoxes;
    int             mPoints;
    int             mKills;
    QString         mName;
    bool            mIsLeader;
};

#endif // PLAYER_H
