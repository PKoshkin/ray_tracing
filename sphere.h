#ifndef SPHERE_KOSHKIN_493 
#define SPHERE_KOSHKIN_493

#include <vector>

#include "figure.h"
#include "vector.h"
#include "point.h"

class Sphere : public virtual Figure {
private:
    double radius;
    Point center;

public:
    Sphere(const Point& inCenter, double radius);
    Sphere(const Sphere& sphere);

    virtual Vector normal(const Point& point) const;
};

Sphere::Sphere(const Point& inCenter, double inRadius) {
    center = inCenter;
    radius = inRadius;
}

Sphere::Sphere(const Sphere& sphere) {
    center = sphere.center;
    radius = sphere.radius;
}

Vector Sphere::normal(const Point& point) const {
    return Vector(point) - Vector(center);
}

#endif
