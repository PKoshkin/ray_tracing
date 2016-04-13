#include <iostream>
#include <vector>
#include <memory>

#include "screen.h"
#include "triangle.h"
#include "sphere.h"
#include "figure.h"
#include "point.h"

int main() {
    Screen screen(500, 500);

    for (int x = 0; x < 100; ++x) {
        for (int y = 0; y < 100; ++y) {
            screen.setColor(x, y, Color(1, 0, 0));
        }
    }

    std::vector< std::unique_ptr<Figure> > figures;
    
    std::unique_ptr<Triangle> trianglePointer(new Triangle(Point(0, 0, 0 ), Point(1, 0, 0), Point(0, 1, 0)));
    std::unique_ptr<Sphere> spherePointer(new Sphere(Point(3, 3, 3), 1));
    figures.push_back(trianglePointer);
    figures.push_back(spherePointer);

    for (int i = 0; i < figures.size(); ++i) {
        std::cout << figures[i]->normal(Point(5, 5, 5)) << " ";
    }
    std::cout << std::endl;

    screen.draw();
    return 0;
}
