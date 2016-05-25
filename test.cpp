#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>

#include "scene.h"
#include "reader.h"
#include "optional.h"
#include "3d_tree.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Not enough arguments!" << std::endl;
        return 0;
    }
    Reader reader(argv[1]);

    Optional<Scene> scene = reader.getScene();
    if (!scene.hasValue()) {
        std::cout << "Error occured!" << std::endl;
        return 0;
    }
    Scene realScene = scene.getValue();

    Tree3D tree(realScene.figures); 

    Optional<Intersection> intersection = tree.getIntersection(Ray(Point(0, 0, 0), Vector(0, 1, 0)));
    if (intersection.hasValue()) {
        std::cout << "Wow!" << std::endl;
    } else {
        std::cout << "No. =(" << std::endl;
    }


/*
    realScene.process(std::atoi(argv[2]), std::atoi(argv[3]));
    realScene.save("not_antianalised.png");
*/
    return 0;
}
