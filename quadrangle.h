#ifndef QUADRANGLE_KOSHKIN_493 
#define QUADRANGLE_KOSHKIN_493

#include <vector>
#include <algorithm>

#include "figure.h"
#include "vector.h"
#include "point.h"
#include "optional.h"
#include "bounding_box.h"
#include "constants.h"
#include "triangle.h"

class Quadrangle : public virtual Figure {
private:
    std::vector<Point> points;
    ColorRGB color;
    double reflection;
    double refraction;
    double alpha;

public:
    Quadrangle(const Point& point1, const Point& point2, const Point& point3, const Point& point4, const ColorRGB& inColor = ColorRGB(1, 0, 0));
    Quadrangle(const Quadrangle& Quadrangle);

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

Quadrangle::Quadrangle(const Point& point1, const Point& point2, const Point& point3, const Point& point4, const ColorRGB& inColor) : color(inColor) {
    points.reserve(3);
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    points.push_back(point4);
}

Quadrangle::Quadrangle(const Quadrangle& quadrangle) {
    for (size_t i = 0; i < points.size(); ++i) {
        points[i] = quadrangle.points[i];
    }
    color = quadrangle.color;
}

double Quadrangle::getReflection() const {
    return reflection;
}

double Quadrangle::getRefraction() const {
    return refraction;
}

double Quadrangle::getAlpha() const {
    return alpha;
}

Vector Quadrangle::normal(const Point& point) const {
    return vectorProduct(Vector(points[1]) - Vector(points[0]), Vector(points[2]) - Vector(points[0])).normalized();
}

void Quadrangle::setColor(const ColorRGB& inColor) {
    color = inColor;
}

ColorRGB Quadrangle::getColor() const {
    return color;
}

Optional<double> Quadrangle::getT(const Ray& ray) const {
    // Барицентрический тест писать для четырехугольника не хочется совсем. Используем таки треугольники
    Triangle triangle1(points[0], points[1], points[2]);
    Triangle triangle2(points[0], points[2], points[3]);
    Optional<double> t1 = triangle1.getT(ray);
    if (t1.hasValue()) {
        return t1;
    } else {
        return triangle2.getT(ray);
    }
}

BoundingBox Quadrangle::boundingBox() const {
    double minX = std::min(points[0].getX(), std::min(points[1].getX(), std::max(points[2].getX(), points[3].getX())));
    double minY = std::min(points[0].getY(), std::min(points[1].getY(), std::max(points[2].getY(), points[3].getY())));
    double minZ = std::min(points[0].getZ(), std::min(points[1].getZ(), std::max(points[2].getZ(), points[3].getZ())));
    double maxX = std::max(points[0].getX(), std::max(points[1].getX(), std::max(points[2].getX(), points[3].getX())));
    double maxY = std::max(points[0].getY(), std::max(points[1].getY(), std::max(points[2].getY(), points[3].getY())));
    double maxZ = std::max(points[0].getZ(), std::max(points[1].getZ(), std::max(points[2].getZ(), points[3].getZ())));
    return BoundingBox(Point(minX, minY, minZ), Point(maxX, maxY, maxZ));
}

std::pair<double, double> Quadrangle::getMinMaxByAxis(size_t axis) const {
    return std::make_pair(
        std::min(points[0][axis], std::min(points[1][axis], std::min(points[2][axis], points[3][axis]))),
        std::max(points[0][axis], std::max(points[1][axis], std::max(points[2][axis], points[3][axis])))
    );
}

void Quadrangle::show() const {
    std::cout << points[0] << " " << points[1] << " " << points[2] << " " << points[3] << " {" << color << "}" << std::endl;
}

#endif
