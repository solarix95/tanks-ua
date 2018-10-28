#ifndef WIND_H
#define WIND_H

class Wind
{
public:
    Wind();

    void  calculate(float speed);

    inline float  force() const { return mForce; }

private:
    float mForce;
    float mDelta;
    float mGradient;
};

#endif // WIND_H
