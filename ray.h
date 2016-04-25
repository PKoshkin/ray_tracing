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
};

Ray::Ray(const Point& inStart, const Vector& inDirection) : start(inStart), direction(inDirection.normalized()) {}

Ray::Ray(const Point& inStart, const Point& inEnd) : start(inStart), direction((Vector(inEnd) - Vector(inStart)).normalized()) {}

Point Ray::getPoint(double t) const {
    return (Vector(start) + t * direction).getEnd();
}

double Ray::getPointT(const Point& point) const {
    return (Vector(point) - Vector(start)).length();
}

#endif
