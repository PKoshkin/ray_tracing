#ifndef BOUNDING_BOX_KOSHKIN_493 
#define BOUNDING_BOX_KOSHKIN_493

#include <algorithm>

#include "point.h"
#include "constants.h"
#include "optional.h"

class BoundingBox {
private:
public:
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
    double volume();

    Optional< std::pair<double, double> > getIntersectionsWithRay(const Ray& ray) const;

    BoundingBox operator+(const BoundingBox& inBox) const;
    BoundingBox& operator=(const BoundingBox& inBox);
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
    if ((maxPoint[mostLengthAxis()] + minPoint[mostLengthAxis()]) / 2 != 0) {
        std::cout << "    middle: " << (maxPoint[mostLengthAxis()] + minPoint[mostLengthAxis()]) / 2 << std::endl;
    }
    return (maxPoint[mostLengthAxis()] + minPoint[mostLengthAxis()]) / 2;
}

double BoundingBox::volume() {
    return (getMaxX() - getMinX()) * (getMaxY() - getMinY()) * (getMaxZ() - getMinZ());
}

Optional< std::pair<double, double> > BoundingBox::getIntersectionsWithRay(const Ray& ray) const {
/*
    return std::make_pair(
        std::min(ray.getXT(minPoint.getX()), std::min(ray.getYT(minPoint.getY()), ray.getZT(minPoint.getZ()))),
        std::max(ray.getXT(maxPoint.getX()), std::max(ray.getYT(maxPoint.getY()), ray.getZT(maxPoint.getZ())))
    );
*/
    double tEnter = -std::numeric_limits<double>::max();
    double tOut = std::numeric_limits<double>::max();
    bool hasEnter = false, hasOut = false;
    for (size_t axis = 0; axis < 3; ++axis) {
        double newEnter = std::min(ray.getCoordinateT(minPoint[axis], axis), ray.getCoordinateT(maxPoint[axis], axis));
        double newOut = std::max(ray.getCoordinateT(minPoint[axis], axis), ray.getCoordinateT(maxPoint[axis], axis));
        if (!hasEnter) {
            tEnter = newOut;
            hasEnter = true;
            continue;
        }
        if (!hasOut) {
            tOut = newOut;
            hasOut = true;
            continue;
        }
        if (newEnter > tEnter) {
            tEnter = newEnter;
        }
        if (newOut < tOut) {
            tOut = newOut;
        }
    }


    if (!hasEnter || !hasOut) {
        return Optional< std::pair<double, double> >();
    }
    if (tOut + EPSILON < tEnter) {
        return Optional< std::pair<double, double> >();
    }

    return Optional< std::pair<double, double> >(std::make_pair(tEnter, tOut));
}

BoundingBox BoundingBox::operator+(const BoundingBox& inBox) const {
    return BoundingBox(
        Point(std::min(getMinX(), inBox.getMinX()), std::min(getMinY(), inBox.getMinY()), std::min(getMinZ(), inBox.getMinZ())),
        Point(std::max(getMaxX(), inBox.getMaxX()), std::max(getMaxY(), inBox.getMaxY()), std::max(getMaxZ(), inBox.getMaxZ()))
    );
}

BoundingBox& BoundingBox::operator=(const BoundingBox& inBox) {
    minPoint = inBox.minPoint;
    maxPoint = inBox.maxPoint;
    return *this;
}

#endif
