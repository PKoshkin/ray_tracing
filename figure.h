#ifndef FIGURE_KOSHKIN_493 
#define FIGURE_KOSHKIN_493

#include <utility>

#include "color.h"
#include "ray.h"
#include "bounding_box.h"
#include "optional.h"

// Интерфейс 3D фигруы, от которой все будут наследоваться
class Figure {
public:
    virtual double getReflection() const = 0;
    virtual double getRefraction() const = 0;
    virtual double getAlpha() const = 0;
    virtual Vector normal(const Point& point) const = 0;
    virtual void setColor(const ColorRGB& inColor) = 0;
    virtual ColorRGB getColor() const = 0;
    virtual Optional<double> getT(const Ray& ray) const = 0; // Возвращает наименьший положительный параметр, если он есть
    virtual BoundingBox boundingBox() const = 0;
    virtual std::pair<double, double> getMinMaxByAxis(size_t axis) const = 0; // Возвращает минимальное и максимальное значение примитива по выбранной оси.

    virtual void show() const = 0;
};

#endif
