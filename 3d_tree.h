#ifndef THE_3D_TREE_KOSHKIN_493 
#define THE_3D_TREE_KOSHKIN_493

#include <memory> // Для умных указателей
#include <vector>

#include "optional.h"
#include "figure.h"
#include "ray.h"

const int MAX_DEPTH = 10;
const int MAX_FIGURES_IN_NODE = 5;

struct Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    BouningBox commonBox;

    int axis; // 0 - x, 1 - y, 2 - z
    double coordinate;

    std::vector< std::shared_ptr<Figure> > figures;

    Node(const std::vector< std::shared_ptr<Figure> >& figures);
    void split(int depth = 0);
    Optional<Intersection> getIntersection(const Ray& ray); // Ищет пересечение рекурсивно в детях
    Optional<Intersection> getIntersectionFromExactNode(const Ray& ray); // Ищет пересечение в конкретной вершине.
};

Node::Node(const std::vector< std::shared_ptr<Figure> >& inFigures) {
    commonBox = (*figures.begin())->boundingBox();
    for (auto it = figures.begin() + 1; it != figures.end(); ++it) {
        commonBox = commonBox + (*it)->boundingBox();
    }
    axis = commonBox.mostLengthAxis();
    coordinate = commonBox.mostLengthAxisMiddle();
}

void Node::split(int depth) {
    if (figures.size() > MAX_FIGURES_IN_NODE) {
        return;
    }
    std::vector< std::shared_ptr<Figure> > leftFigures, rightFigures;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        // Смотрим, в какакую часть разбиения попал каждый примитив и добавляем его туда.
        if (coordinate < (*it)->boundingBox().getMinPoint()[axis]) {
            leftFigures.push_back(*it);
        } else if (coordinate > (*it)->boundingBox().getMaxPoint()[axis]) {
            rightFigures.push_back(*it);
        } else {
            // Некоторые примитивы добавляются в обе области
            leftFigures.push_back(*it);
            rightFigures.push_back(*it);
        }
    }
    left = std::make_shared<Node>(leftFigures);
    right = std::make_shared<Node>(rightFigures);
    figures.clear();
    if (depth <= MAX_DEPTH) {
        left->split(depth + 1);
        right->split(depth + 1);
    }
}

Optional<Intersection> Node::getIntersectionFromExactNode(const Ray& ray) {
    std::shared_ptr<Figure> intersectingFigurePointer;
    double t; // Параметр луча, отвечающий точке пересечения
    bool foundIntersections = false;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        Optional<double> tempT = (*it)->getT(ray);
        if (tempT.hasValue()) {
            if (!foundIntersections) {
                intersectingFigurePointer = *it;
                t = tempT.getValue();
                foundIntersections = true;
                continue;
            } else {
                if (tempT.getValue() < t) {
                    intersectingFigurePointer = *it;
                    t = tempT.getValue();
                }
            }
        }
    }
    if (foundIntersection) {
        return Optional<Intersection>(intersectingFigurePointer, t);
    } else {
        return Optional<Intersection>();
    }
}

Optional<Intersection> Node::getIntersection(const Ray& ray) {
    if (!figures.empty()) {
        // Дошли до листа, пора искать пересечения с фигурами
        return getIntersectionFromThisNode(ray);
    }
    std::pair<double, double> boxT = commonBox.getIntersectionsWithRay(ray);
    double splitT = ray.getCoordinateT(coordinate, axis);
    if ((splitT < boxT.first) || (splitT > boxT.second)) {
        // Пересечений у луча с боксом нет.
        return Optional<Intersection>();
    }
    Optinal<Intersection> intersection = left->getIntersection(ray);
    if (intersection.hasValue()) {
        return intersection;
    } else {
        return right->getIntersection(ray);
    }
}

class Tree3D {
private:
    std::shared_ptr<Node> root;
    BoundingBox commonBox;

public:
    Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures);
    Optional<Intersection> getIntersection(const Ray& ray);
};

Tree3D::Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures) {
    // Считаем общий ограничивающий прямоугольник и делим корень дерева
    root = std::make_shared<Node>(inFigures);
    commonBox = (*inFigures.begin())->boundingBox();
    for (auto it = inFigures.begin() + 1; it != inFigures.end(); ++it) {
        commonBox = commonBox + (*it)->boundingBox();
    }
    root->split();
}

Optioanl<Intersection> Tree3D::getFugure(const Ray& ray) {
    return root->getIntersection(ray);
}

#endif
