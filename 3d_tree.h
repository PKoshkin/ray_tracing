#ifndef THE_3D_TREE_KOSHKIN_493 
#define THE_3D_TREE_KOSHKIN_493

#include <memory> // Для умных указателей
#include <vector>

#include "optional.h"
#include "figure.h"
#include "ray.h"
#include "bounding_box.h"
#include "intersection.h"


#include <iostream>


const int MAX_DEPTH = 10;
const int MAX_FIGURES_IN_NODE = 1;

struct Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    BoundingBox commonBox;

    int axis; // 0 - x, 1 - y, 2 - z
    double coordinate;
    bool hasLeft, hasRight;

    std::vector< std::shared_ptr<Figure> > figures;

    Node(const std::vector< std::shared_ptr<Figure> >& figures);
    void split(int depth = 0);
    Optional<Intersection> getIntersection(const Ray& ray); // Ищет пересечение рекурсивно в детях
    Optional<Intersection> getIntersectionFromExactNode(const Ray& ray); // Ищет пересечение в конкретной вершине.
};

Node::Node(const std::vector< std::shared_ptr<Figure> >& inFigures) : figures(inFigures), hasLeft(false), hasRight(false) {
    commonBox = (*figures.begin())->boundingBox();
    for (auto it = figures.begin() + 1; it != figures.end(); ++it) {
        commonBox = commonBox + (*it)->boundingBox();
    }
    axis = commonBox.mostLengthAxis();
    coordinate = commonBox.mostLengthAxisMiddle();
}

void Node::split(int depth) {

    std::cout << "split figure of size=" << figures.size() << std::endl;

    if (figures.size() < MAX_FIGURES_IN_NODE) {
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

    figures.clear();
    if (!leftFigures.empty()) {

        std::cout << "    has left!" << std::endl;

        left = std::make_shared<Node>(leftFigures);
        hasLeft = true;
        if (depth <= MAX_DEPTH) {
            left->split(depth + 1);
        } else {
            std::cout << "new leaf!" << std::endl;
        }
    }
    if (!rightFigures.empty()) {

        std::cout << "    has right!" << std::endl;

        right = std::make_shared<Node>(rightFigures);
        hasRight = true;
        if (depth <= MAX_DEPTH) {
            right->split(depth + 1);
        } else {
            std::cout << "new leaf!" << std::endl;
        }
    }
}

Optional<Intersection> Node::getIntersectionFromExactNode(const Ray& ray) {
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

Optional<Intersection> Node::getIntersection(const Ray& ray) {
    if (!hasLeft && !hasRight) {
        // Дошли до листа, пора искать пересечения с фигурами
        return getIntersectionFromExactNode(ray);
    }
    std::pair<double, double> boxT = commonBox.getIntersectionsWithRay(ray);
    double splitT = ray.getCoordinateT(coordinate, axis);
    if ((splitT < boxT.first) || (splitT > boxT.second)) {
        // Пересечений у луча с боксом нет.
        return Optional<Intersection>();
    }
    if (hasLeft) {
        Optional<Intersection> intersection = left->getIntersection(ray);
        if (intersection.hasValue()) {
            return intersection;
        } else {
            if (hasRight) {
                return right->getIntersection(ray);
            } else {
                return Optional<Intersection>();
            }
        }
    } else {
        // Правый сын здесь точно есть
        return right->getIntersection(ray);
    }
}

class Tree3D {
private:
    std::shared_ptr<Node> root;

public:
    Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures);
    Optional<Intersection> getIntersection(const Ray& ray);
};

Tree3D::Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures) : root(new Node(inFigures)) {
    // Делим корень дерева
    root->split();
}

Optional<Intersection> Tree3D::getIntersection(const Ray& ray) {
    return root->getIntersection(ray);
}

#endif
