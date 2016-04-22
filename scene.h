#ifndef SCENE_KOSHKIN_493 
#define SCENE_KOSHKIN_493

#include <vector>
#include <memory>
#include <cairo.h>
#include <math.h>

#include "vector.h"
#include "pixel.h"
#include "figure.h"
#include "point.h"
#include "color.h"
#include "constants.h"
#include "lighter.h"

class Scene {
private:
    
    // Разрешение экрана
    int height;
    int width;

    double distanceToScreen; // Расстояние от камеры до экрана
    Point camera; // Точка в которой стоит камера
    Vector direction; // Направление камеры

    // Базис в плоскости экрана
    Vector axisX;
    Vector axisY;

    std::vector< std::vector<Pixel> > pixels;
    std::vector< std::unique_ptr<Figure> > figures;
    std::vector< std::unique_ptr<Lighter> > lighters;

public:
    Scene(int inWidth, int inHeight, double inDistanceToScreen, Point inCamera, Vector inDirection, Vector inAxisX, Vector inAxisY);
    void setColor(int x, int y, const ColorRGB& color);
    void addFigure(std::unique_ptr<Figure> figure);
    void addLighter(const Point& point);
    Vector vectorToPixel(const Pixel& pixel) const;
    ColorRGB runRay(const Pixel& pixel) const;
    void process();
    bool reaches(const Point& begin, const Point& end) const;
    void draw() const;
};

Scene::Scene(int inWidth, int inHeight, double inDistanceToScreen, Point inCamera, Vector inDirection, Vector inAxisX, Vector inAxisY) :
    height(inHeight), width(inWidth), distanceToScreen(inDistanceToScreen), camera(inCamera), direction(inDirection), axisX(inAxisX), axisY(inAxisY) {
    direction.normalize();
    axisX.normalize();
    axisY.normalize();

    pixels.reserve(width);
    for (int i = 0; i < width; ++i) {
        pixels.push_back(std::vector<Pixel>());
        pixels[i].reserve(height);
        for (int j = 0; j < height; ++j) {
            pixels[i].push_back(Pixel(i, j));
        }
    }
}

void Scene::addFigure(std::unique_ptr<Figure> figure) {
    figures.push_back(std::move(figure));
}

void Scene::addLighter(const Point& point) {
    lighters.emplace_back(new Lighter(point));
}

void Scene::setColor(int x, int y, const ColorRGB& color) {
    pixels[x][y].setColor(color);
}

Vector Scene::vectorToPixel(const Pixel& pixel) const {
    return Vector(camera) + direction * distanceToScreen + axisX * (pixel.getX() - width / 2) + axisY * (pixel.getY() - height / 2);
}

ColorRGB Scene::runRay(const Pixel& pixel) const {
    Vector rayDirection = vectorToPixel(pixel);
    Point pixelPoint = rayDirection.getEnd();
    rayDirection.normalize();

    Point closestIntersection;
    ColorRGB color;
    bool foundIntersections = false;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        Optional<Point> intersection = (*it)->intersectionWithRay(pixelPoint, rayDirection);
        if (intersection.hasValue()) {
            if (!foundIntersections) {
                closestIntersection = intersection.getValue();
                color = (*it)->getColor();
                color.normalize(fabs(scalarProduct(rayDirection, (*it)->normal(closestIntersection))));
                foundIntersections = true;
                continue;
            } else {
                if ((distance(pixelPoint, closestIntersection) > distance(pixelPoint, intersection.getValue()))) {
                    closestIntersection = intersection.getValue();
                    color = (*it)->getColor();
                    color.normalize(fabs(scalarProduct(rayDirection, (*it)->normal(closestIntersection))));
                }
            }
        }
    }
    if (foundIntersections) {
        double intensity = 0;
        for (auto it = lighters.begin(); it != lighters.end(); ++it) {
/*
            for (auto jt = lighters.begin(); jt != lighters.end(); ++jt) {
                if ((*it)->lightesToPoint()) {
                    
                }
            }
*/
            intensity += (*it)->intensity(closestIntersection);
        }

        return color;
    } else {
        return ColorRGB();
    }
}

/*
bool Scene::reaches(const Point& begin, const Point& end) const {
    Vector direction = (Vector(end) - Vector(begin)).normalized();
    for (auto it = lighters.begin(); it != lighters.end(); ++it) {

    }
}
*/
    
void Scene::process() {
    for (int x = 0; x < width; ++x) {
        std::cout << x << std::endl;
        for (int y = 0; y < height; ++y) {
            pixels[x][y].setColor(runRay(pixels[x][y]));
        }
    }
}

void Scene::draw() const {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);
    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            cairo_set_source_rgb(cr, pixels[i][j].getRed(), pixels[i][j].getGreen(), pixels[i][j].getBlue());
            cairo_rectangle(cr, pixels[i][j].getX(), pixels[i][j].getY(), 1, 1);
            cairo_fill(cr);
        }
    }

    cairo_surface_write_to_png(surface, "out.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

#endif
