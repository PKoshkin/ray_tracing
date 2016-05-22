#ifndef LIGHTER_KOSHKIN_493 
#define LIGHTER_KOSHKIN_493

#include "point.h"
#include "constants.h"

class Lighter {
private:
    Point place;
    double power;

public:
    Lighter(const Lighter& lighter);
    Lighter(const Point& point, double inPower = INTENSITY_CONSTANT);
    double intensity(const Point& point) const;
    Point getPlace() const;

    double getPower() const;
    void setPower(double inConstant);
};

Lighter::Lighter(const Lighter& lighter) : place(lighter.place), power(lighter.power) {}
Lighter::Lighter(const Point& inPlace, double inPower) : place(inPlace), power(inPower) {}

double Lighter::intensity(const Point& point) const {
    return power / ((distance(place, point)) * (distance(place, point)));
}

Point Lighter::getPlace() const {
    return place;
}

double Lighter::getPower() const {
    return power;
}

void Lighter::setPower(double inConstant) {
    power = inConstant;
}

#endif
