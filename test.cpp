#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "scene.h"
#include "reader.h"
#include "optional.h"

int main(int argc, char** argv) {
    if (argc < 2) {
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
    realScene.process();
    realScene.save("not_antianalised.png");

    return 0;
}
