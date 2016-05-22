#ifndef TRIANGLE_KOSHKIN_493 
#define TRIANGLE_KOSHKIN_493

#include <vector>
#include <algorithm>

#include "figure.h"
#include "vector.h"
#include "point.h"
#include "optional.h"
#include "plane.h"
#include "bounding_box.h"
#include "constants.h"

class Triangle : public virtual Figure {
private:
public:
    std::vector<Point> points;
    ColorRGB color;

public:
    Triangle(const Point& point1, const Point& point2, const Point& point3, const ColorRGB& inColor = ColorRGB(1, 0, 0));
    Triangle(const Triangle& triangle);

    double area() const;
    Plane getPlane() const;

    virtual Vector normal(const Point& point) const;
    virtual bool touchesPoint(const Point& point) const;
    virtual void setColor(const ColorRGB& inColor);
    virtual ColorRGB getColor() const;
    virtual Optional<double> getT(const Ray& ray) const;
    virtual BoundingBox boundingBox() const;

    virtual void show() const;
};

Triangle::Triangle(const Point& point1, const Point& point2, const Point& point3, const ColorRGB& inColor) : color(inColor) {
    points.reserve(3);
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
}

Triangle::Triangle(const Triangle& triangle) {
    for (size_t i = 0; i < points.size(); ++i) {
        points[i] = triangle.points[i];
    }
    color = triangle.color;
}

Vector Triangle::normal(const Point& point) const {
    return vectorProduct(Vector(points[1]) - Vector(points[0]), Vector(points[2]) - Vector(points[0])).normalized();
}

double Triangle::area() const {
    return vectorProduct(Vector(points[1]) - Vector(points[0]), Vector(points[2]) - Vector(points[0])).length() / 2;
}

Plane Triangle::getPlane() const {
    return Plane(points[0], points[1], points[2]); 
}

bool Triangle::touchesPoint(const Point& point) const {
    return (fabs(Triangle(points[0], points[1], point).area() + Triangle(points[1], points[2], point).area() + Triangle(points[2], points[0], point).area() - area()) < EPSILON);
}

void Triangle::setColor(const ColorRGB& inColor) {
    color = inColor;
}

ColorRGB Triangle::getColor() const {
    return color;
}

Optional<double> Triangle::getT(const Ray& ray) const {
    // Проводим барицентрический тест
    Vector edge1 = Vector(points[1]) - Vector(points[0]);
    Vector edge2 = Vector(points[2]) - Vector(points[0]);

    Vector P = vectorProduct(ray.direction, edge2);
    double det = scalarProduct(P, edge1);
    if (fabs(det) < EPSILON) {
        return Optional<double>();
    }
    double inv_det = 1.0 / det;
    Vector move = Vector(ray.start) - Vector(points[0]);

    double u = scalarProduct(move, P) * inv_det;
    if (u < 0.0 || u > 1.0) {
        return Optional<double>();
    }
    Vector Q = vectorProduct(move, edge1);
    double v = scalarProduct(ray.direction, Q) * inv_det;
    if(v < 0.0 || u + v  > 1.0) {
        return Optional<double>();
    }
    double t = scalarProduct(edge2, Q) * inv_det;
    if (t > 0) {
        return Optional<double>(t);
    } else {
        return Optional<double>();
    }
}

BoundingBox Triangle::boundingBox() const {
    double minX = std::min(points[0].getX(), std::min(points[1].getX(), points[2].getX()));
    double minY = std::min(points[0].getY(), std::min(points[1].getY(), points[2].getY()));
    double minZ = std::min(points[0].getZ(), std::min(points[1].getZ(), points[2].getZ()));
    double maxX = std::max(points[0].getX(), std::max(points[1].getX(), points[2].getX()));
    double maxY = std::max(points[0].getY(), std::max(points[1].getY(), points[2].getY()));
    double maxZ = std::max(points[0].getZ(), std::max(points[1].getZ(), points[2].getZ()));
    return BoundingBox(Point(minX, minY, minZ), Point(maxX, maxY, maxZ));
}

void Triangle::show() const {
    std::cout << points[0] << " " << points[1] << " " << points[2] << " {" << color << "}" << std::endl;
}

#endif
