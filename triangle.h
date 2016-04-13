#ifndef TRIANGLE_KOSHKIN_493 
#define TRIANGLE_KOSHKIN_493

#include <vector>

#include "figure.h"
#include "vector.h"
#include "point.h"

class Triangle : public virtual Figure {
private:
    std::vector<Point> points;

public:
    Triangle(const Point& point1, const Point& point2, const Point& point3);
    Triangle(const Triangle& triangle);

    virtual Vector normal(const Point& point) const;
};

Triangle::Triangle(const Point& point1, const Point& point2, const Point& point3) {
    points.reserve(3);
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
}

Triangle::Triangle(const Triangle& triangle) {
    for (int i = 0; i < points.size(); ++i) {
        points[i] = triangle.points[i];
    }
}

Vector Triangle::normal(const Point& point) const {
    return vectorProduct(Vector(points[0]) - Vector(points[1]), Vector(points[0]) - Vector(points[2]));
}

#endif
