#ifndef FIGURE_KOSHKIN_493 
#define FIGURE_KOSHKIN_493

#include "vector.h"
#include "point.h"

// Интерфейс 3D фигруы, от которой все будут наследоваться
class Figure {
public:
    virtual Vector normal(const Point& point) const = 0;
};

#endif
