#ifndef READER_KROSHKIN_493 
#define READER_KROSHKIN_493

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include "scene.h"
#include "optional.h"
#include "point.h"
#include "color_rgb.h"
#include "lighter.h"
#include "figure.h"
#include "constants.h"
#include "plane.h"
#include "sphere.h"
#include "triangle.h"
#include "quadrangle.h"

struct Material {
    std::string name;
    ColorRGB color;
    double reflect, refract, alpha;

    Material() {}
};

struct SceneData {
    Point origin, topLeft, bottomLeft, topRight;
    double lightPower, lightDistance;
    std::vector<Material> materials;
    std::vector< std::pair<Point, double> > lighters;
    std::vector< std::pair<std::shared_ptr<Figure>, std::string> > figures;

    SceneData() {}
};

class Reader {
private:
    std::fstream file;
    SceneData sceneData;

    std::stringstream line;
    std::string word;

public:
    Reader(const std::string fileName);

    //Optional<Scene> getScene();
    int processScene(int width, int height, const char* fileName);

    int read();

    int readViewport(); // Считываем описание экрана

    int readMaterials();
    int readMaterial();

    int readLights();
    int readLightPoint();
    int readLightReference();

    int readGeometry();
    int readSphere();
    int readTriangle();
    int readQuadrangle();

    void getWordFromNewLine();

    void showData() const {
        std::cout << "origin: " << sceneData.origin << " top left: " << sceneData.topLeft << " bottomLeft: " << sceneData.bottomLeft << " top right: " << sceneData.topRight << std::endl;
        std::cout << "materials: " << sceneData.materials.size() << " lighters: " << sceneData.lighters.size() << " figures: " << sceneData.figures.size() << std::endl;
    }
};

Reader::Reader(const std::string fileName) : file(fileName, std::ios::in) {}

int Reader::processScene(int screenWidth, int screenHeight, const char* fileName) {
    if (read() == -1) {
        return -1;
    } else {
        double width = distance(sceneData.topRight, sceneData.topLeft);
        double height = distance(sceneData.topLeft, sceneData.bottomLeft);
        Vector xAxis = (Vector(sceneData.topRight) - Vector(sceneData.topLeft)).normalized();
        Vector yAxis = (Vector(sceneData.topLeft) - Vector(sceneData.bottomLeft)).normalized();
        Vector toCenter = Vector(sceneData.topRight) - Vector(sceneData.origin) - (width / 2) * xAxis - (height / 2) * yAxis;
        Scene scene(
            width,
            height,
            toCenter,
            sceneData.origin,
            xAxis,
            yAxis
        );

        double lightConstant = sceneData.lightDistance * sceneData.lightDistance / sceneData.lightPower;
        for (auto it = sceneData.lighters.begin(); it != sceneData.lighters.end(); ++it) {
            scene.addLighter(Lighter(it->first, lightConstant * it->second));
        }

        std::vector< std::shared_ptr<Figure> > figures;
        for (auto it = sceneData.figures.begin(); it != sceneData.figures.end(); ++it) {
            for (auto jt = sceneData.materials.begin(); jt != sceneData.materials.end(); ++jt) {
                if (jt->name == it->second) {
                    it->first->setColor(jt->color);
                    figures.push_back(std::move(it->first));
                    break;
                }
            }
        }

        Tree3D tree(figures);
        scene.setTree(tree);
        scene.process(screenWidth, screenHeight);
        scene.whiteBalance();
        scene.save(fileName);
        return 0;
    }
}

void Reader::getWordFromNewLine() {
    std::string lineString;
    std::getline(file, lineString);
    line = std::stringstream(lineString);
    line >> word;
}

int Reader::readViewport() {
    while (word != "endviewport") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "origin") {
            line >> sceneData.origin;
        } else if (word == "topleft") {
            line >> sceneData.topLeft;
        } else if (word == "bottomleft") {
            line >> sceneData.bottomLeft;
        } else if (word == "topright") {
            line >> sceneData.topRight;
        }
    } 
    return 0;
}

int Reader::readMaterial() {
    Material newMaterial;
    while (word != "endentry") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "name") {
            line >> newMaterial.name;
        } else if (word == "color") {
            line >> newMaterial.color;
        } else if (word == "reflect") {
            line >> newMaterial.reflect;
        } else if (word == "refract") {
            line >> newMaterial.refract;
        } else if (word == "alpha") {
            line >> newMaterial.alpha;
        }
    } 
    sceneData.materials.push_back(newMaterial);
    return 0;
}

int Reader::readMaterials() {
    while (word != "endmaterials") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "entry") {
            readMaterial();
        }
    } 
    return 0;
}

int Reader::readLights() {
    while (word != "endlights") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "reference") {
            if (readLightReference() == -1) {
                return -1;
            }
        } else if (word == "point") {
            if (readLightPoint() == -1) {
                return -1;
            }
        }
    } 
    return 0;
}

int Reader::readLightPoint() {
    Point place;
    double power;
    while (word != "endpoint") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "power") {
            line >> power;
        } else if (word == "coords") {
            line >> place;
        }
    }
    sceneData.lighters.push_back(std::make_pair(place, power));
    return 0;
}

int Reader::readLightReference() {
    while (word != "endreference") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "distance") {
            line >> sceneData.lightDistance;
        } else if (word == "power") {
            line >> sceneData.lightPower;
        }
    }
    return 0;
}

int Reader::readGeometry() {
    while (word != "endgeometry") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "sphere") {
            if (readSphere() == -1) {
                return -1;
            }
        } else if (word == "triangle") {
            if (readTriangle() == -1) {
                return -1;
            }
        } else if (word == "quadrangle") {
            if (readQuadrangle() == -1) {
                return -1;
            }
        }
    } 
    return 0;
}

int Reader::readSphere() {
    Point center;
    double radius;
    std::string materialName;
    while (word != "endsphere") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "coords") {
            line >> center;
        } else if (word == "radius") {
            line >> radius;
        } else if (word == "material") {
            line >> materialName;
        }
    } 
    std::shared_ptr<Sphere> newSphere(new Sphere(center, radius));
    sceneData.figures.push_back(std::make_pair(newSphere, materialName));
    return 0;
}

int Reader::readTriangle() {
    Point points[3];
    int index = 0;
    std::string materialName;
    while (word != "endtriangle") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "vertex") {
            line >> points[index];
            ++index;
        } else if (word == "material") {
            line >> materialName;
        }
    } 
    std::shared_ptr<Triangle> newTriangle(new Triangle(points[0], points[1], points[2]));
    sceneData.figures.push_back(std::make_pair(newTriangle, materialName));
    return 0;
}

int Reader::readQuadrangle() {
    Point points[4];
    int index = 0;
    std::string materialName;
    while (word != "endquadrangle") {
        if (!file) {
            // Файл закончился без закрытия блока
            return -1;
        }
        getWordFromNewLine();

        if (word == "vertex") {
            line >> points[index];
            ++index;
        } else if (word == "material") {
            line >> materialName;
        }
    } 
    std::shared_ptr<Quadrangle> newQuadrangle(new Quadrangle(points[0], points[1], points[2], points[3]));
    sceneData.figures.push_back(std::make_pair(newQuadrangle, materialName));
    return 0;
}

int Reader::read() {
    while (file) {
        getWordFromNewLine();

        if (word == "viewport") {
            if (readViewport() == -1) {
                return -1;
            }
        } else if (word == "materials") {
            if (readMaterials() == -1) {
                return -1;
            }
        } else if (word == "lights") {
            if (readLights() == -1) {
                return -1;
            }
        } else if (word == "geometry") {
            if (readGeometry() == -1) {
                return -1;
            }
        }
    } 
    return 0;
}

#endif
