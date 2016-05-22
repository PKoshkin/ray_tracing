#ifndef THE_3D_TREE_KOSHKIN_493 
#define THE_3D_TREE_KOSHKIN_493

#include <memory> // Для умных указателей
#include <vector>

#include "figure.h"
#include "ray.h"

const int MAX_DEPTH = 10;
const int MAX_FIGURES_IN_NODE = 5;

struct Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    int axis; // 0 - x, 1 - y, 2 - z
    double coordinate;

    std::vector< std::shared_ptr<Figure> > figures;

    Node(const std::vector< std::shared_ptr<Figure> >& figures);
    void split(int depth = 0);

    // Является ли вершина листом. Если вершина не лист, примитивы складывать в неё не будем.
    bool isLeaf() const;
};

Node::Node(const std::vector< std::shared_ptr<Figure> >& inFigures) {
    BoundingBox commonBox = (*figures.begin())->boundingBox();
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
    left = std::make_unique<Node>(leftFigures);
    right = std::make_unique<Node>(rightFigures);
    figures.clear();
    if (depth <= MAX_DEPTH) {
        left->split(depth + 1);
        right->split(depth + 1);
    }
}

bool Node::isLeaf() const {
    return !figures.empty();
}

class Tree3D {
private:
    std::unique_ptr<Node> root;
    BoundingBox commonBox;

public:
    Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures);
    std::shared_ptr<Figure> getFugure(const Ray& ray);
};

Tree3D::Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures) {
    // Считаем общий ограничивающий прямоугольник и делим корень дерева
    root = std::make_unique<Node>(inFigures);
    commonBox = (*inFigures.begin())->boundingBox();
    for (auto it = inFigures.begin() + 1; it != inFigures.end(); ++it) {
        commonBox = commonBox + (*it)->boundingBox();
    }
    root->split();
}

std::shared_ptr<Figure> getFugure(const Ray& ray) {
    double nearT, farT;
    

}

#endif
