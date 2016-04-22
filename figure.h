#ifndef FIGURE_KOSHKIN_493 
#define FIGURE_KOSHKIN_493

#include "color.h"
#include "ray.h"
#include "optional.h"

// Интерфейс 3D фигруы, от которой все будут наследоваться
class Figure {
public:
    virtual Vector normal(const Point& point) const = 0;
    virtual bool touchesPoint(const Point& point) const = 0;
    virtual Optional<Point> intersectionWithRay(const Ray& ray) const = 0;
    virtual ColorRGB getColor() const = 0;
};

#endif
