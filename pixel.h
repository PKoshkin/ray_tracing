#ifndef PIXEL_KOSHKIN_493 
#define PIXEL_KOSHKIN_493

#include <math.h>

#include "color.h"

class Pixel {
private:
    int x;
    int y;
    ColorRGB color;
    
public:
    Pixel(int inX, int inY);
    Pixel(int inX, int inY, const ColorRGB& inColor);
    void setColor(const ColorRGB& inColor);
    int getX() const;
    int getY() const;
    double getRed() const;
    double getGreen() const;
    double getBlue() const;
};

Pixel::Pixel(int inX, int inY) : x(inX), y(inY), color() {}

Pixel::Pixel(int inX, int inY, const ColorRGB& inColor) : x(inX), y(inY), color(inColor) {}

void Pixel::setColor(const ColorRGB& inColor) {
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

double distance(const Point& point1, const Point& point2) {
    return sqrt(
            (point1.getX() - point2.getX()) * (point1.getX() - point2.getX()) +
            (point1.getY() - point2.getY()) * (point1.getY() - point2.getY()) +
            (point1.getZ() - point2.getZ()) * (point1.getZ() - point2.getZ())
    );
}

#endif
