#ifndef SPHERE_KOSHKIN_493 
#define SPHERE_KOSHKIN_493

#include <vector>
#include <math.h>

#include "figure.h"
#include "vector.h"
#include "point.h"
#include "constants.h"
#include "color.h"
#include "optional.h"

class Sphere : public virtual Figure {
private:
    double radius;
    Point center;
    ColorRGB color;

public:
    Sphere(const Point& inCenter, double radius, const ColorRGB& inColor);
    Sphere(const Sphere& sphere);

    virtual Vector normal(const Point& point) const;
    virtual bool touchesPoint(const Point& point) const;
    virtual Optional<Point> intersectionWithRay(const Ray& ray) const;
    virtual ColorRGB getColor() const;
};

Sphere::Sphere(const Point& inCenter, double inRadius, const ColorRGB& inColor) : center(inCenter), radius(inRadius), color(inColor) {}

Sphere::Sphere(const Sphere& sphere) : center(sphere.center), radius(sphere.radius), color(sphere.color) {}

Vector Sphere::normal(const Point& point) const {
    return (Vector(point) - Vector(center)).normalized();
}

bool Sphere::touchesPoint(const Point& point) const {
    return fabs(distance(point, center) - radius) < EPSILON;
}

Optional<Point> Sphere::intersectionWithRay(const Ray& ray) const {
    double A = ray.direction.squareLength();
    double B = 2 * scalarProduct(ray.direction, Vector(ray.start) - Vector(center));
    double C = (Vector(ray.start) - Vector(center)).squareLength() - radius * radius;

    if ((B * B - 4 * A * C) < 0) {
        return Optional<Point>();
    } else {
        // Ближней к пикселю будет точка с меньшим положительным параметром
        double t1 = (-B - sqrt(B * B - 4 * A * C)) / (2 * A);
        double t2 = (-B + sqrt(B * B - 4 * A * C)) / (2 * A);

        if (t1 > 0) {
            return Optional<Point>((Vector(ray.start) + t1 * ray.direction).getEnd());
        } else {
            return Optional<Point>();
        }
    }
}

ColorRGB Sphere::getColor() const {
    return color;
}
#endif

