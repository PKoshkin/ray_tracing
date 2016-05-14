#ifndef THE_3D_TREE_KOSHKIN_493 
#define THE_3D_TREE_KOSHKIN_493

#include <memory> // Для unique_ptr
#include <vector>

#include "figure.h"

struct Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    int axis; // 0 - x, 1 - y, 2 - z
    double coordinate;

    std::vector< std::shared_ptr<Figure> > figures;

    Node(int inAxis, double inCoordinate, const std::vector< std::shared_ptr<Figure> >& figures);
    void split();

    // Является ли вершина листом. Если вершина не лист, примитивы складывать в неё не будем.
    bool isLeaf() const;
};

Node::Node(int inAxis, double inCoordinate, const std::vector< std::shared_ptr<Figure> >& inFigures) :
    axis(inAxis), coordinate(inCoordinate), figures(inFigures) {}

void Node::split(int splitCoordinate) {
    std::vector

}

bool Node::isLeaf() const {
    return !figures.empty();
}

class Tree3D {
private:
    std::unique_ptr<Node> root;

public:
    Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures); 
};

Tree3D::Tree3D(const std::vector< std::shared_ptr<Figure> >& inFigures) { 
    root = std::unique_ptr<Node>(new Node(0, 0, inFigures));
    root->split();
}

#endif
