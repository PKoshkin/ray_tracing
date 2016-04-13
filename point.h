#ifndef POINT_KOSHKIN_493 
#define POINT_KOSHKIN_493

class Point {
private:
    double x;
    double y;
    double z;

public:
    Point() {};
    Point(double inX, double inY, double inZ);
    Point(const Point& point);

    double getX() const;
    double getY() const;
    double getZ() const;
};

Point::Point(double inX, double inY, double inZ) : x(inX), y(inY), z(inZ) {}

Point::Point(const Point& point) : x(point.x), y(point.y), z(point.z) {}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double Point::getZ() const {
    return z;
}

#endif
