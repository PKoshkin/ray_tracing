#ifndef TRIANGLE_KOSHKIN_493 
#define TRIANGLE_KOSHKIN_493

#include <vector>

#include "figure.h"
#include "vector.h"
#include "point.h"
#include "optional.h"
#include "sheet.h"

class Triangle : public virtual Figure {
private:
    std::vector<Point> points;
    ColorRGB color;

public:
    Triangle(const Point& point1, const Point& point2, const Point& point3, const ColorRGB& inColor = ColorRGB());
    Triangle(const Triangle& triangle);

    double area() const;

    virtual Vector normal(const Point& point) const;
    virtual bool touchesPoint(const Point& point) const;
    virtual Optional<Point> intersectionWithRay(const Point& start, const Vector& direction) const;
    virtual ColorRGB getColor() const;
};

Triangle::Triangle(const Point& point1, const Point& point2, const Point& point3, const ColorRGB& inColor) : color(inColor) {
    points.reserve(3);
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
}

Triangle::Triangle(const Triangle& triangle) {
    for (int i = 0; i < points.size(); ++i) {
        points[i] = triangle.points[i];
    }
    color = triangle.color;
}

Vector Triangle::normal(const Point& point) const {
    return vectorProduct(Vector(points[1]) - Vector(points[0]), Vector(points[2]) - Vector(points[0])).normalized();
}

double Triangle::area() const {
    return vectorProduct(Vector(points[0]) - Vector(points[1]), Vector(points[0]) - Vector(points[2])).length() / 2;
}

bool Triangle::touchesPoint(const Point& point) const {
    return (fabs(Triangle(points[0], points[1], point).area() + Triangle(points[1], points[2], point).area() + Triangle(points[2], points[0], point).area() - area()) < EPSILON);
}

Optional<Point> Triangle::intersectionWithRay(const Point& start, const Vector& direction) const {
    Optional<Point> sheetIntersection = Sheet(points[0], points[1], points[2]).intersectionWithRay(start, direction);
    if (sheetIntersection.hasValue()) {
        if (touchesPoint(sheetIntersection.getValue())) {
            return Optional<Point>(sheetIntersection.getValue());
        } else {
            return Optional<Point>();
        }
    } else {
        return Optional<Point>();
    }
}

ColorRGB Triangle::getColor() const {
    return color;
}

#endif
