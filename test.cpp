#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "scene.h"
#include "triangle.h"
#include "sphere.h"
#include "figure.h"
#include "point.h"
#include "vector.h"
#include "ray.h"
#include "lighter.h"
#include "3d_tree.h"

#include "optional.h"

int main() {
    Scene scene(1000, 1000, 1000, Point(0, 0, 0), Vector(0, 1, 0), Vector(1, 0, 0), Vector(0, 0, -1));

    std::unique_ptr<Triangle> tr(new Triangle(Point(-900, 2000, -900), Point(900, 2300, -900), Point(900, 2000, 900), ColorRGB(0, 1, 0)));
    std::unique_ptr<Triangle> tr1(new Triangle(Point(-900, 2000, -900), Point(-900, 2300, 900), Point(900, 2000, 900), ColorRGB(1, 1, 0)));
    std::unique_ptr<Triangle> tr2(new Triangle(Point(0, 1200, 0), Point(600, 1200, 0), Point(0, 1200, 600), ColorRGB(1, 0, 0)));
    std::unique_ptr<Sphere> sp(new Sphere(Point(300, 1500, 0), 100, ColorRGB(1, 0, 0)));
    std::unique_ptr<Sphere> sp1(new Sphere(Point(0, 1500, 300), 100, ColorRGB(0, 1, 0)));
    std::unique_ptr<Sphere> sp2(new Sphere(Point(300, 1500, 300), 100, ColorRGB(0, 0, 1)));

    scene.addFigure(std::move(tr));
    scene.addFigure(std::move(tr1));
    //scene.addFigure(std::move(tr2));
    scene.addFigure(std::move(sp));
    scene.addFigure(std::move(sp1));
    scene.addFigure(std::move(sp2));

    scene.addLighter(Lighter(Point(300, 1000, 0), INTENSITY_CONSTANT / 2));
    scene.addLighter(Lighter(Point(0, 1000, 300), INTENSITY_CONSTANT / 2));
    scene.addLighter(Lighter(Point(300, 1000, 300), INTENSITY_CONSTANT / 2));
    scene.addLighter(Lighter(Point(-300, 1000, 0), INTENSITY_CONSTANT / 2));
    scene.addLighter(Lighter(Point(0, 1000, -300), INTENSITY_CONSTANT / 2));
    scene.addLighter(Lighter(Point(-300, 1000, -300), INTENSITY_CONSTANT / 2));

    scene.process();
    scene.save("not_antianalised.png");
/*
    scene.antialiase();
    scene.save("antianalised_x1.png");
    scene.antialiase();
    scene.save("antianalised_x2.png");
    scene.antialiase();
    scene.save("antianalised_x3.png");
*/
    return 0;
}
