#include "particle.h"

Particle::Particle() : id(0), origX(0), origY(0),
                       destX(0), destY(0), speed(0),
                       red(0), green(0), blue(0)
{

}

void Particle::setId(int id) { this->id = id; }
void Particle::setOrigX(int origX) { this->origX = origX; }
void Particle::setOrigY(int origY) { this->origY = origY; }
void Particle::setDestX(int destX) { this->destX = destX; }
void Particle::setDestY(int destY) { this->destY = destY; }
void Particle::setSpeed(int speed) { this->speed = speed; }
void Particle::setRed(short red) { this->red = red; }
void Particle::setGreen(short green) { this->green = green; }
void Particle::setBlue(short blue) { this->blue = blue; }

int Particle::getId() const { return id; }
int Particle::getOrigX() const { return origX; }
int Particle::getOrigY() const { return origY; }
int Particle::getDestX() const { return destX; }
int Particle::getDestY() const { return destY; }
int Particle::getSpeed() const { return speed; }
short Particle::getRed() const { return red; }
short Particle::getGreen() const { return green; }
short Particle::getBlue() const { return blue; }

double Particle::computeEuclideanDist(double orgX, double orgY, double destX, double destY)
{
    double distance = sqrt(pow(destX - orgX, 2) + pow(destY - orgY, 2));
    return distance;
}
