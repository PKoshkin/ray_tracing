#ifndef SHEET_KOSHKIN_493 
#define SHEET_KOSHKIN_493

#include "optional.h"
#include "point.h"

class Sheet {
private:
    double A, B, C, D;

public:
    Sheet(double inA, double inB, double inC, double inD);
    Sheet(const Point& point1, const Point& point2, const Point& point3);

    Optional<Point> intersectionWithRay(const Point& start, const Vector& direction) const;
};

Sheet::Sheet(double inA, double inB, double inC, double inD) : A(inA), B(inB), C(inC), D(inC) {}

Sheet::Sheet(const Point& point1, const Point& point2, const Point& point3) {
    A = (point2.getY() - point1.getY()) * (point3.getZ() - point1.getZ()) - (point3.getY() - point1.getY()) * (point2.getZ() - point1.getZ());
    B = (point2.getX() - point1.getX()) * (point3.getZ() - point1.getZ()) - (point3.getX() - point1.getX()) * (point2.getZ() - point1.getZ());
    C = (point2.getX() - point1.getX()) * (point3.getY() - point1.getY()) - (point3.getX() - point1.getX()) * (point2.getY() - point1.getY());
    D = -(A * point1.getX() + B * point1.getY() + C * point1.getZ());
}

Optional<Point> Sheet::intersectionWithRay(const Point& start, const Vector& direction) const {
    if (A * direction.getEnd().getX() + B * direction.getEnd().getY() + C * direction.getEnd().getZ() == 0) {
        return Optional<Point>();
    } else {
        double t = -(A * start.getX() + B * start.getY() + C * start.getZ() + D) / (A * direction.getEnd().getX() + B * direction.getEnd().getY() + C * direction.getEnd().getZ());
        return Optional<Point>((Vector(start) + t * direction).getEnd());
    }
}

#endif
