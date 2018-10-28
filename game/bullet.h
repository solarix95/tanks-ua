#ifndef BULLET_H
#define BULLET_H

struct Bullet {
    enum GunType {
        StandardShoot,
        MediumShoot,     // Einzelschuss, kräftiger als Standard
        ScatterShoot,    // Vervielfälltigung am Scheitelpunkt
        HammerShoot,     // Vervielfälltigung beim Aufschlag
        AtomicShoot      // Gewaltige Kraft, jedoch instabil..
    };

    GunType type;
    int     strength;
    int     radius;
    Bullet(GunType t, int s, int r) : type(t), strength(s), radius(r) {}
};

#endif // BULLET_H
