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
    double reflection;
    double refraction;
    double alpha;

public:
    Sphere(const Point& inCenter, double radius, const ColorRGB& inColor = ColorRGB());
    Sphere(const Sphere& sphere);

    virtual double getReflection() const;
    virtual double getRefraction() const;
    virtual double getAlpha() const;
    virtual Vector normal(const Point& point) const;
    virtual void setColor(const ColorRGB& inColor);
    virtual ColorRGB getColor() const;
    virtual Optional<double> getT(const Ray& ray) const;
    virtual BoundingBox boundingBox() const;
    virtual std::pair<double, double> getMinMaxByAxis(size_t axis) const;

    virtual void show() const;
};

Sphere::Sphere(const Point& inCenter, double inRadius, const ColorRGB& inColor) : radius(inRadius), center(inCenter), color(inColor) {}

Sphere::Sphere(const Sphere& sphere) : radius(sphere.radius), center(sphere.center), color(sphere.color) {}

double Sphere::getReflection() const {
    return reflection;
}

double Sphere::getRefraction() const {
    return refraction;
}

double Sphere::getAlpha() const {
    return alpha;
}

Vector Sphere::normal(const Point& point) const {
    return (Vector(point) - Vector(center)).normalized();
}

void Sphere::setColor(const ColorRGB& inColor) {
    color = inColor;
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

std::pair<double, double> Sphere::getMinMaxByAxis(size_t axis) const {
    return std::make_pair(center[axis] - radius, center[axis] + radius);
}

void Sphere::show() const {
    std::cout << center << " " << radius << " {" << color << "}" << std::endl;
}

#endif
