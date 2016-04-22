#include <iostream>
#include <vector>
#include <memory>

#include "scene.h"
#include "triangle.h"
#include "sphere.h"
#include "figure.h"
#include "point.h"
#include "vector.h"
#include "ray.h"
#include "3d_tree.h"

#include "optional.h"

int main() {
    Scene scene(1000, 1000, 100, Point(0, 0, 0), Vector(0, 1, 0), Vector(1, 0, 0), Vector(0, 0, 1));

    std::unique_ptr<Triangle> trianglePointer(new Triangle(Point(-800, 210, -800), Point(-850, 210, 850), Point(800, 210, 850), ColorRGB(0, 1, 0)));
    std::unique_ptr<Sphere> spherePointer(new Sphere(Point(-600, 450, 0), 300, ColorRGB(0, 1, 0)));
    std::unique_ptr<Sphere> spherePointer1(new Sphere(Point(300, 450, 0), 300, ColorRGB(0, 1, 0)));

    scene.addFigure(std::move(trianglePointer));
    scene.addFigure(std::move(spherePointer));
    scene.addFigure(std::move(spherePointer1));

    //scene.addLighter(Point(0, 20, 0));

    scene.process();
    scene.draw();

    return 0;
}
