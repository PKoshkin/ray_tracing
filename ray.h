#ifndef RAY_KOSHKIN_493 
#define RAY_KOSHKIN_493

#include "vector.h"
#include "point.h"

struct Ray {
    Point start;
    Vector direction;

    Ray(const Point& inStart, const Vector& inDirection);
    Ray(const Point& inStart, const Point& inEnd);
    Point getPoint(double t) const;
    double getPointT(const Point& point) const;
    double getCoordinateT(double coordinate, int axis) const;
    double getXT(double x) const;
    double getYT(double y) const;
    double getZT(double z) const;
};

Ray::Ray(const Point& inStart, const Vector& inDirection) : start(inStart), direction(inDirection.normalized()) {}

Ray::Ray(const Point& inStart, const Point& inEnd) : start(inStart), direction((Vector(inEnd) - Vector(inStart)).normalized()) {}

Point Ray::getPoint(double t) const {
    return (Vector(start) + t * direction).getEnd();
}

double Ray::getPointT(const Point& point) const {
    return (Vector(point) - Vector(start)).length();
}

double Ray::getCoordinateT(double coordinate, int axis) const {
    return (coordinate - start[axis]) / direction[axis];
}

double Ray::getXT(double x) const {
    return getCoordinateT(x, 0); 
}

double Ray::getYT(double y) const {
    return getCoordinateT(y, 1);
}

double Ray::getZT(double z) const {
    return getCoordinateT(z, 2);
}

#endif
