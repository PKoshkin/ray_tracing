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
    reader.processScene(std::atoi(argv[2]), std::atoi(argv[3]), "not_antialiace.png");
    return 0;
}
