#ifndef PLANE_KOSHKIN_493 
#define PLANE_KOSHKIN_493

#include "optional.h"
#include "ray.h"

class Plane {
private:
    double A, B, C, D;

public:
    Plane(double inA, double inB, double inC, double inD);
    Plane(const Point& point1, const Point& point2, const Point& point3);

    Optional<double> getT(const Ray& ray) const;
};

Plane::Plane(double inA, double inB, double inC, double inD) : A(inA), B(inB), C(inC), D(inD) {}

Plane::Plane(const Point& point1, const Point& point2, const Point& point3) {
    A = (point2.getY() - point1.getY()) * (point3.getZ() - point1.getZ()) - (point3.getY() - point1.getY()) * (point2.getZ() - point1.getZ());
    B = (point2.getX() - point1.getX()) * (point3.getZ() - point1.getZ()) - (point3.getX() - point1.getX()) * (point2.getZ() - point1.getZ());
    C = (point2.getX() - point1.getX()) * (point3.getY() - point1.getY()) - (point3.getX() - point1.getX()) * (point2.getY() - point1.getY());
    D = -(A * point1.getX() + B * point1.getY() + C * point1.getZ());
}

Optional<double> Plane::getT(const Ray& ray) const {
    if (A * ray.direction.getEnd().getX() + B * ray.direction.getEnd().getY() + C * ray.direction.getEnd().getZ() == 0) {
        return Optional<double>();
    } else {
        double t = -(A * ray.start.getX() + B * ray.start.getY() + C * ray.start.getZ() + D) /
            (A * ray.direction.getEnd().getX() + B * ray.direction.getEnd().getY() + C * ray.direction.getEnd().getZ());
        return Optional<double>(t);
    } 
}

#endif
