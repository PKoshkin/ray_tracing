#ifndef BOUNDING_BOX_KOSHKIN_493 
#define BOUNDING_BOX_KOSHKIN_493

#include <algorithm>

#include "point.h"

class BoundingBox {
private:
    Point minPoint;
    Point maxPoint;

public:
    BoundingBox();
    BoundingBox(const Point& inMinPoint, const Point& inMaxPoint);
    BoundingBox(const BoundingBox& boundingBox);

    Point getMinPoint() const;
    Point getMaxPoint() const;
    double getMinX() const;
    double getMinY() const;
    double getMinZ() const;
    double getMaxX() const;
    double getMaxY() const;
    double getMaxZ() const;
    int mostLengthAxis() const;
    double mostLengthAxisMiddle() const;

    BoundingBox operator+(const BoundingBox& inBox) const;
};

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const Point& inMinPoint, const Point& inMaxPoint) : minPoint(inMinPoint), maxPoint(inMaxPoint) {}

BoundingBox::BoundingBox(const BoundingBox& boundingBox) : minPoint(boundingBox.minPoint), maxPoint(boundingBox.maxPoint) {}

Point BoundingBox::getMinPoint() const {
    return minPoint;
}

Point BoundingBox::getMaxPoint() const {
    return maxPoint;
}

double BoundingBox::getMinX() const {
    return minPoint.getX();
}

double BoundingBox::getMinY() const {
    return minPoint.getY();
}

double BoundingBox::getMinZ() const {
    return minPoint.getZ();
}

double BoundingBox::getMaxX() const {
    return maxPoint.getX();
}

double BoundingBox::getMaxY() const {
    return maxPoint.getY();
}

double BoundingBox::getMaxZ() const {
    return maxPoint.getZ();
}

int BoundingBox::mostLengthAxis() const {
    if ((maxPoint.getX() - minPoint.getX() > maxPoint.getY() - minPoint.getY()) && (maxPoint.getX() - minPoint.getX() > maxPoint.getZ() - minPoint.getZ())) {
        return 0;
    } else if ((maxPoint.getY() - minPoint.getY() > maxPoint.getX() - minPoint.getX()) && (maxPoint.getY() - minPoint.getY() > maxPoint.getZ() - minPoint.getZ())) {
        return 1;
    } else {
        return 2;
    }
}

double BoundingBox::mostLengthAxisMiddle() const {
    return maxPoint[mostLengthAxis()] - minPoint[mostLengthAxis()];
}

BoundingBox BoundingBox::operator+(const BoundingBox& inBox) const {
    return BoundingBox(
        Point(std::min(getMinX(), inBox.getMinX()), std::min(getMinY(), inBox.getMinY()), std::min(getMinZ(), inBox.getMinZ())),
        Point(std::max(getMaxX(), inBox.getMaxX()), std::max(getMaxY(), inBox.getMaxY()), std::max(getMaxZ(), inBox.getMaxZ()))
    );
}

#endif
