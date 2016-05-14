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
    virtual ColorRGB getColor() const;
    virtual Optional<double> getT(const Ray& ray) const;
    virtual BoundingBox boundingBox() const;
};

Sphere::Sphere(const Point& inCenter, double inRadius, const ColorRGB& inColor) : radius(inRadius), center(inCenter), color(inColor) {}

Sphere::Sphere(const Sphere& sphere) : radius(sphere.radius), center(sphere.center), color(sphere.color) {}

Vector Sphere::normal(const Point& point) const {
    return (Vector(point) - Vector(center)).normalized();
}

bool Sphere::touchesPoint(const Point& point) const {
    return fabs(distance(point, center) - radius) < EPSILON;
}

ColorRGB Sphere::getColor() const {
    return color;
}

Optional<double> Sphere::getT(const Ray& ray) const {
    double A = ray.direction.squareLength();
    double B = 2 * scalarProduct(ray.direction, Vector(ray.start) - Vector(center));
    double C = (Vector(ray.start) - Vector(center)).squareLength() - radius * radius;

    if ((B * B - 4 * A * C) < 0) {
        return Optional<double>();
    } else {
        // Ближней к пикселю будет точка с меньшим положительным параметром
        double t1 = (-B - sqrt(B * B - 4 * A * C)) / (2 * A);
        double t2 = (-B + sqrt(B * B - 4 * A * C)) / (2 * A);

        if (t1 > 0) {
            return Optional<double>(t1);
        } else if (t2 > 0) {
            return Optional<double>(t2);
        } else {
            return Optional<double>();
        }
    }
}

BoundingBox Sphere::boundingBox() const {
    return BoundingBox(
        Point(center.getX() - radius, center.getY() - radius, center.getZ() - radius),
        Point(center.getX() + radius, center.getY() + radius, center.getZ() + radius)
    );
}

#endif
