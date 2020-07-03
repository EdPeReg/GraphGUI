#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
public:
    Particle();

    void setId(int id);
    void setOrigX(int origX);
    void setOrigY(int origY);
    void setDestX(int destX);
    void setDestY(int destY);
    void setSpeed(int speed);
    void setRed(short red);
    void setGreen(short green);
    void setBlue(short blue);

    int getId() const;
    int getOrigX() const;
    int getOrigY() const;
    int getDestX() const;
    int getDestY() const;
    int getSpeed() const;
    short getRed() const;
    short getGreen() const;
    short getBlue() const;

private:
    int id;
    int origX, origY;
    int destX, destY;
    int speed;
    short red, green, blue;
};

#endif // PARTICLE_H
