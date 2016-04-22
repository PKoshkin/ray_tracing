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

    std::shared_ptr< std::vector< std::unique_ptr<Figure> > > figures;


    Node(int inAxis, double inCoordinate, std::shared_ptr< std::vector< std::unique_ptr<Figure> > > inFigures);

    bool isSheet();
};

Node::Node(int inAxis, double inCoordinate, std::shared_ptr< std::vector< std::unique_ptr<Figure> > > inFigures) : axis(inAxis), coordinate(inCoordinate) {

}

bool Node::isSheet() {
    return bool(figures);
}

class Tree3D {
private:
    std::unique_ptr<Node> root;

public:
    Tree3D(std::shared_ptr< std::vector< std::unique_ptr<Figure> > > inFigures); 
};

Tree3D::Tree3D(std::shared_ptr< std::vector< std::unique_ptr<Figure> > > inFigures) { 
    root = std::unique_ptr<Node>(new Node(0, 0, inFigures));
/*
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        
    }
*/
}

#endif
