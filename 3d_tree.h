#ifndef 3D_TREE_KOSHKIN_493 
#define 3D_TREE_KOSHKIN_493

#include <memory> // Для unique_ptr

#include "figure.h"

struct Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    int type; // 0 - x, 1 - y, 2 - z
    double coordinate;

    std::unique_ptr< std::vector<Figure> > figures;
};

class 3DTree {
private:
    Node* root;

public:
    
};

#endif
