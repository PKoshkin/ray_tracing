#ifndef PIXEL_KOSHKIN_493 
#define PIXEL_KOSHKIN_493

#include "color.h"

class Pixel {
private:
    int x;
    int y;
    Color color;
    
public:
    Pixel(int inX, int inY);
    Pixel(int inX, int inY, const Color& inColor);
    void setColor(const Color& inColor);
    int getX() const;
    int getY() const;
    double getRed() const;
    double getGreen() const;
    double getBlue() const;
};

Pixel::Pixel(int inX, int inY) : x(inX), y(inY), color() {}

Pixel::Pixel(int inX, int inY, const Color& inColor) : x(inX), y(inY), color(inColor) {}

void Pixel::setColor(const Color& inColor) {
    color = inColor;
}

int Pixel::getX() const {
    return x;
}

int Pixel::getY() const {
    return y;
}

double Pixel::getRed() const {
    return color.getRed();
}

double Pixel::getGreen() const {
    return color.getGreen();
}

double Pixel::getBlue() const {
    return color.getBlue();
}

#endif
