#ifndef BOUNDING_BOX_KOSHKIN_493 
#define BOUNDING_BOX_KOSHKIN_493

#include "point.h"

class BoundingBox {
private:
    Point minPoint;
    Point maxPoint;

public:
    BoundingBox(const Point& inMinPoint, const Point& inMaxPoint);
};

BoundingBox::BoundingBox(const Point& inMinPoint, const Point& inMaxPoint) : minPoint(inMinPoint), maxPoint(inMaxPoint) {}

#endif
