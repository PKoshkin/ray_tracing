#ifndef FIGURE_KOSHKIN_493 
#define FIGURE_KOSHKIN_493

#include "color.h"
#include "ray.h"
#include "bounding_box.h"
#include "optional.h"

// Интерфейс 3D фигруы, от которой все будут наследоваться
class Figure {
public:
    virtual Vector normal(const Point& point) const = 0;
    virtual bool touchesPoint(const Point& point) const = 0;
    virtual ColorRGB getColor() const = 0;
    virtual Optional<double> getT(const Ray& ray) const = 0; // Возвращает наименьший положительный параметр, если он есть
    virtual BoundingBox boundingBox() const = 0;
};

#endif
