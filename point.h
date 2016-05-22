#ifndef POINT_KOSHKIN_493 
#define POINT_KOSHKIN_493

#include <fstream>

#include "math.h"

class Point {
private:
    double x;
    double y;
    double z;

public:
    Point() {};
    Point(double inX, double inY, double inZ);
    Point(const Point& point);

    void operator=(const Point& point);
    double operator[](const int index) const;
    friend std::istream& operator>>(std::istream& in, Point& point);
    friend std::ostream& operator<<(std::ostream& out, const Point& point);

    double getX() const;
    double getY() const;
    double getZ() const;
};

Point::Point(double inX, double inY, double inZ) : x(inX), y(inY), z(inZ) {}

Point::Point(const Point& point) : x(point.x), y(point.y), z(point.z) {}

void Point::operator=(const Point& point) {
    x = point.x;
    y = point.y;
    z = point.z;
}

double Point::operator[](const int index) const {
    // (x, y, z) - занумерованы в таком порядке 
    if (index == 0) {
        return x;
    } else if (index == 1) {
        return y;
    } else if (index == 2) {
        return z;
    }
}

std::istream& operator>>(std::istream& in, Point& point) {
    in >> point.x >> point.y >> point.z;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
    out << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return out;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double Point::getZ() const {
    return z;
}

double distance(const Point& point1, const Point& point2) {
    return sqrt(
            (point1.getX() - point2.getX()) * (point1.getX() - point2.getX()) +
            (point1.getY() - point2.getY()) * (point1.getY() - point2.getY()) +
            (point1.getZ() - point2.getZ()) * (point1.getZ() - point2.getZ())
    );
}

#endif
