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


const int MAX_DEPTH = 8;
const int MAX_FIGURES_IN_NODE = 1;

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
/*
    std::cout << "---------------------------------\n";
*/
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
/*
    std::cout << "SAH = " << surfaceAreaHeuristic << " coordinate: " << coordinate << " axis: " << axis << std::endl;
    std::cout << "---------------------------------\n";
*/
}

double Node::calculateSurfaceAreaHeuristic(double inCoordinate, int inAxis) {
    size_t leftN = 0, rightN = 0;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        // Смотрим, в какакую часть разбиения попал каждый примитив и прибавляем единичку соответствующему счетчику
        if (inCoordinate >= (*it)->boundingBox().getMaxPoint()[inAxis]) {
            ++leftN;
        } else if (inCoordinate <= (*it)->boundingBox().getMinPoint()[inAxis]) {
            ++rightN;
        } else {
            // Некоторые примитивы добавляются в обе области
            ++leftN;
            ++rightN;
        }
    }
    double leftVolume = 1, rightVolume = 1;
    for (int i = 0; i < 3; ++i) {
        if (i != axis) {
            leftVolume *= commonBox.maxPoint[i] - commonBox.minPoint[i];
            rightVolume *= commonBox.maxPoint[i] - commonBox.minPoint[i];
        } else {
            leftVolume *= fabs(inCoordinate - commonBox.minPoint[i]);
            rightVolume *= fabs(commonBox.maxPoint[i] - inCoordinate);
        }

/*
        std::cout << "calculating volume: " << commonBox.maxPoint[i] - commonBox.minPoint[i] << " "
            << commonBox.maxPoint[i] - commonBox.minPoint[i] << " " <<
            inCoordinate - commonBox.minPoint[i] << " " <<
            commonBox.maxPoint[i] - inCoordinate << std::endl;
*/


    }

//    std::cout << leftVolume << " * " << leftN << " + " << rightVolume << " * " << rightN << " = " << "new SAH: " << leftVolume * leftN + rightVolume * rightN << std::endl;

    return leftVolume * leftN + rightVolume * rightN;
}

void Node::split(int depth) {

//    std::cout << "split: coordinate: " << coordinate << " axis: " << axis << " " << commonBox.minPoint << " " << commonBox.maxPoint << std::endl;

    // Посчитаем SAH и проверим, надо ли было в вамом деле сплититься
    if (commonBox.volume() * figures.size() < surfaceAreaHeuristic) {
        return;
    }
    std::vector< std::shared_ptr<Figure> > leftFigures, rightFigures;

    std::cout << "-------------------------------\n";
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        // Смотрим, в какакую часть разбиения попал каждый примитив и добавляем его туда.
        if (coordinate >= (*it)->boundingBox().getMaxPoint()[axis]) {
            std::cout << "    left" << std::endl;
            leftFigures.push_back(*it);
        } else if (coordinate <= (*it)->boundingBox().getMinPoint()[axis]) {
            std::cout << "    right" << std::endl;
            rightFigures.push_back(*it);
        } else {
            // Некоторые примитивы добавляются в обе области
            std::cout << "    left and right" << std::endl;
            leftFigures.push_back(*it);
            rightFigures.push_back(*it);
        }
    }
    std::cout << "-------------------------------\n";

    std::cout << "    left size: " << leftFigures.size() << " right size: " << rightFigures.size() << std::endl;

    figures.clear();
    if (!leftFigures.empty()) {
        left = std::make_shared<Node>(leftFigures);
        hasLeft = true;
    }
    if (!rightFigures.empty()) {
        right = std::make_shared<Node>(leftFigures);
        hasRight = true;
    }
    if ((leftFigures.size() >= MAX_FIGURES_IN_NODE) && (depth <= MAX_DEPTH)) {
        left->split(depth + 1);
    }
    if ((rightFigures.size() >= MAX_FIGURES_IN_NODE) && (depth <= MAX_DEPTH)) {
        right->split(depth + 1);
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
        std::cout << "stop" << std::endl;
        return getIntersectionFromExactNode(ray);
    }
    std::pair<double, double> boxT = commonBox.getIntersectionsWithRay(ray);
    double splitT = ray.getCoordinateT(coordinate, axis);

//    std::cout << boxT.first << " " << splitT << " " << boxT.second << std::endl;
    

    if ((splitT < boxT.first) || (splitT > boxT.second)) {
        // Пересечений у луча с боксом нет.
        std::cout << "no intersection" << std::endl;
        return Optional<Intersection>();
    }

    // Сначала пойдем в ребенка, в котором лежит начало луча
    std::shared_ptr<Node> first, second;
    bool hasFirst, hasSecond;
    if (ray.start[axis] < coordinate) {

//        std::cout << "fisrt - left" << std::endl;

        first = left;
        second = right;
        hasFirst = hasLeft;
        hasSecond = hasRight;
    } else {

//        std::cout << "fisrt - right" << std::endl;

        first = right;
        second = left;
        hasFirst = hasRight;
        hasSecond = hasLeft;
    }
    if (hasFirst) {
        Optional<Intersection> intersection = first->getIntersection(ray);
        if (intersection.hasValue()) {
            std::cout << "-> first";
            return intersection;
        } else {
            if (hasSecond) {
                std::cout << "-> second";
                return second->getIntersection(ray);
            } else {
                std::cout << "-> nothing";
                return Optional<Intersection>();
            }
        }
    } else {
        std::cout << "-> second (no first)";
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
