#ifndef THE_3D_TREE_KOSHKIN_493 
#define THE_3D_TREE_KOSHKIN_493

#include <memory> // Для умных указателей
#include <vector>
#include <limits>
#include <math.h>

#include "optional.h"
#include "figure.h"
#include "ray.h"
#include "bounding_box.h"
#include "intersection.h"


#include <iostream>


const int MAX_DEPTH = 5;
const int MAX_FIGURES_IN_NODE = 3;

struct Node {
    std::shared_ptr<Node> left;//
    std::shared_ptr<Node> right;
    BoundingBox commonBox;

    int axis; // 0 - x, 1 - y, 2 - z
    double coordinate;
    bool hasLeft, hasRight;
    double surfaceAreaHeuristic;

    std::vector< std::shared_ptr<Figure> > figures;

    Node(const std::vector< std::shared_ptr<Figure> >& figures);
    void split(int depth = 0);
    Optional<Intersection> getIntersection(const Ray& ray) const; // Ищет пересечение рекурсивно в детях
    Optional<Intersection> getIntersectionFromExactNode(const Ray& ray) const; // Ищет пересечение в конкретной вершине.
    double calculateSurfaceAreaHeuristic(double inCoordinate, int inAxis);


    void show(int depth);
};

Node::Node(const std::vector< std::shared_ptr<Figure> >& inFigures) : figures(inFigures), hasLeft(false), hasRight(false) {
    commonBox = (*figures.begin())->boundingBox();
    for (auto it = figures.begin() + 1; it != figures.end(); ++it) {
        commonBox = commonBox + (*it)->boundingBox();
    }

    surfaceAreaHeuristic = std::numeric_limits<double>::max();
    for (size_t currentAxis = 0; currentAxis < 3; ++currentAxis) {
        for (auto it = figures.begin(); it != figures.end(); ++it) {
            std::pair<double, double> coordinates = (*it)->getMinMaxByAxis(axis);
            double minSAH = calculateSurfaceAreaHeuristic(coordinates.first, currentAxis);
            double maxSAH = calculateSurfaceAreaHeuristic(coordinates.second, currentAxis);
            if (minSAH < surfaceAreaHeuristic) {
                surfaceAreaHeuristic = minSAH;
                axis = currentAxis;
                coordinate = coordinates.first;
            }
            if (maxSAH < surfaceAreaHeuristic) {
                surfaceAreaHeuristic = maxSAH;
                axis = currentAxis;
                coordinate = coordinates.second;
            }
        }
    }
}

double Node::calculateSurfaceAreaHeuristic(double inCoordinate, int inAxis) {
    size_t leftN = 0, rightN = 0;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        // Смотрим, в какакую часть разбиения попал каждый примитив и прибавляем единичку соответствующему счетчику
        if (inCoordinate >= (*it)->boundingBox().getMinPoint()[inAxis] - EPSILON) {
            ++leftN;
        }
        if (inCoordinate <= (*it)->boundingBox().getMaxPoint()[inAxis] + EPSILON) {
            ++rightN;
        }
    }
    double leftSides[3], rightSides[3];
    for (int currentAxis = 0; currentAxis < 3; ++currentAxis) {
        if (currentAxis != axis) {
            rightSides[currentAxis] = commonBox.maxPoint[currentAxis] - commonBox.minPoint[currentAxis];
            rightSides[currentAxis] = commonBox.maxPoint[currentAxis] - commonBox.minPoint[currentAxis];
        } else {
            leftSides[currentAxis] = fabs(inCoordinate - commonBox.minPoint[currentAxis]);
            rightSides[currentAxis] = fabs(commonBox.maxPoint[currentAxis] - inCoordinate);
        }
    }
    double leftArea = 2 * (leftSides[0] * leftSides[1] + leftSides[1] * leftSides[2] + leftSides[2] * leftSides[0]);
    double rightArea = 2 * (rightSides[0] * rightSides[1] + rightSides[1] * rightSides[2] + rightSides[2] * rightSides[0]);
    return leftArea * leftN + rightArea * rightN;
}

void Node::split(int depth) {
    // Посчитаем SAH и проверим, надо ли было в вамом деле сплититься
    if (commonBox.getArea() * figures.size() < surfaceAreaHeuristic + EPSILON) {
        return;
    }
    if ((figures.size() <= MAX_FIGURES_IN_NODE) || (depth >= MAX_DEPTH)) {
        return;
    }
    std::vector< std::shared_ptr<Figure> > leftFigures, rightFigures;

    for (auto it = figures.begin(); it != figures.end(); ++it) {
        // Смотрим, в какакую часть разбиения попал каждый примитив и добавляем его туда.
        if (coordinate >= (*it)->boundingBox().getMinPoint()[axis] - EPSILON) {
            leftFigures.push_back(*it);
        }
        if (coordinate <= (*it)->boundingBox().getMaxPoint()[axis] + EPSILON) {
            rightFigures.push_back(*it);
        }
    }

    figures.clear();
    if (!leftFigures.empty()) {
        left = std::make_shared<Node>(leftFigures);
        left->split(depth + 1);
        hasLeft = true;
    }
    if (!rightFigures.empty()) {
        right = std::make_shared<Node>(rightFigures);
        right->split(depth + 1);
        hasRight = true;
    }
}

Optional<Intersection> Node::getIntersectionFromExactNode(const Ray& ray) const {
    std::shared_ptr<Figure> intersectingFigurePointer;
    double t; // Параметр луча, отвечающий точке пересечения
    bool foundIntersection = false;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        Optional<double> tempT = (*it)->getT(ray);
        if (tempT.hasValue()) {
            if (!foundIntersection) {
                intersectingFigurePointer = *it;
                t = tempT.getValue();
                foundIntersection = true;
            } else {
                if (tempT.getValue() < t) {
                    intersectingFigurePointer = *it;
                    t = tempT.getValue();
                }
            }
        }
    }
    if (foundIntersection) {
        return Optional<Intersection>(Intersection(intersectingFigurePointer, t));
    } else {
        return Optional<Intersection>();
    }
}

Optional<Intersection> Node::getIntersection(const Ray& ray) const {
    if (!hasLeft && !hasRight) {
        // Дошли до листа, пора искать пересечения с фигурами
        return getIntersectionFromExactNode(ray);
    }
    Optional< std::pair<double, double> > boxT = commonBox.getIntersectionsWithRay(ray);
    if (!boxT.hasValue()) {
        return Optional<Intersection>();
    }
    double splitT = ray.getCoordinateT(coordinate, axis);

    // Сначала пойдем в ребенка, в котором лежит начало луча
    std::shared_ptr<Node> first, second;
    bool hasFirst, hasSecond;
    if (ray.start[axis] < coordinate) {
        first = left;
        second = right;
        hasFirst = hasLeft;
        hasSecond = hasRight;
    } else {
        first = right;
        second = left;
        hasFirst = hasRight;
        hasSecond = hasLeft;
    }

    if (splitT <= boxT.getValue().first) {
        if (hasSecond) {
            return second->getIntersection(ray);
        } else {
            return Optional<Intersection>();
        }
    }
    if (splitT >= boxT.getValue().second) {
        if (hasFirst) {
            return first->getIntersection(ray);
        } else {
            return Optional<Intersection>();
        }
    }

    if (hasFirst) {
        Optional<Intersection> intersection = first->getIntersection(ray);
        if (intersection.hasValue()) {
            return intersection;
        } else {
            if (hasSecond) {
                return second->getIntersection(ray);
            } else {
                return Optional<Intersection>();
            }
        }
    } else {
        // Правый сын здесь точно есть
        return second->getIntersection(ray);
    }
}

class Tree3D {
private:
    std::shared_ptr<Node> root;

public:
    Tree3D() {}
    Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures);
    Tree3D(const Tree3D& tree);
    Optional<Intersection> getIntersection(const Ray& ray) const;
    Tree3D& operator=(const Tree3D& inTree);


    void show();
};

Tree3D::Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures) : root(new Node(inFigures)) {
    // Делим корень дерева
    root->split();
}

Tree3D::Tree3D(const Tree3D& tree) : root(tree.root) {}

Optional<Intersection> Tree3D::getIntersection(const Ray& ray) const {
    return root->getIntersection(ray);
}

Tree3D& Tree3D::operator=(const Tree3D& tree) {
    root = tree.root;
    return *this;
}

void Tree3D::show() {
    root->show(0);
}

void Node::show(int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << figures.size() << std::endl;
    if (hasLeft) { 
        left->show(depth + 1);
    }
    if (hasRight) { 
        right->show(depth + 1);
    }
}

#endif
