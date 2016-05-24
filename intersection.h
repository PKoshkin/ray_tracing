#ifndef INTERSECTION_KOSHKIN_493 
#define INTERSECTION_KOSHKIN_493

#include <memory>

#include "figure.h"

struct Instersection {
    double t; // Параметр луча, соответствующий точке пересечения
    std::shared_ptr<Figure> figure;

    Intersection(std::shared_ptr<Figure> inFigure, double inT);
};

Intersection::Intersection(std::shared_ptr<Figure> inFigure, double inT) : figure(inFigure), t(inT) {}

#endif
