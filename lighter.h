#ifndef LIGHTER_KOSHKIN_493 
#define LIGHTER_KOSHKIN_493

#include "point.h"
#include "constants.h"

class Lighter {
private:
    Point place;
public:

    Lighter(const Point& point);
    double intensity(const Point& point) const;
    Point getPlace() const;
};

Lighter::Lighter(const Point& inPlace) : place(inPlace) {}

double Lighter::intensity(const Point& point) const {
    return INTENSITY_CONSTANT / ((distance(place, point)) * (distance(place, point)));
}

Point Lighter::getPlace() const {
    return place;
}


#endif
