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
#include "ray.h"

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
    std::vector< std::shared_ptr<Figure> > figures;
    std::vector<Lighter> lighters;

public:
    Scene() {}
    Scene(int inWidth, int inHeight, double inDistanceToScreen, Point inCamera, Vector inDirection, Vector inAxisX, Vector inAxisY);
    void addFigure(std::shared_ptr<Figure> figure);
    void addLighter(const Lighter& inLighter);
    Vector vectorToPixel(const Pixel& pixel) const;
    ColorRGB runRay(const Pixel& pixel) const;
    void process();
    void save(const char* file) const;
    void antialiase();
};

Scene::Scene(int inWidth, int inHeight, double inDistanceToScreen, Point inCamera, Vector inDirection, Vector inAxisX, Vector inAxisY) :
    height(inHeight), width(inWidth), distanceToScreen(inDistanceToScreen), camera(inCamera), direction(inDirection.normalized()), axisX(inAxisX.normalized()), axisY(inAxisY.normalized()) {

    pixels.reserve(width);
    for (size_t i = 0; i < width; ++i) {
        pixels.push_back(std::vector<Pixel>());
        pixels[i].reserve(height);
        for (size_t j = 0; j < height; ++j) {
            pixels[i].push_back(Pixel(i, j));
        }
    }
}

void Scene::addFigure(std::shared_ptr<Figure> figure) {
    figures.emplace_back(std::move(figure));
}

void Scene::addLighter(const Lighter& inLighter) {
    lighters.push_back(inLighter);
}

Vector Scene::vectorToPixel(const Pixel& pixel) const {
    return Vector(camera) + direction * distanceToScreen + axisX * (pixel.getX() - width / 2) + axisY * (pixel.getY() - height / 2);
}

ColorRGB Scene::runRay(const Pixel& pixel) const {
    Ray ray(vectorToPixel(pixel).getEnd(), vectorToPixel(pixel));

    // Ищем фигуру, с которой пересекается луч
    std::shared_ptr<Figure> intersectingFigurePointer;
    double t; // Параметр луча, отвечающий точке пересечения
    bool foundIntersections = false;
    for (auto it = figures.begin(); it != figures.end(); ++it) {
        Optional<double> tempT = (*it)->getT(ray);
        if (tempT.hasValue()) {
            if (!foundIntersections) {
                intersectingFigurePointer = *it;
                t = tempT.getValue();
                foundIntersections = true;
                continue;
            } else {
                if (tempT.getValue() < t) {
                    intersectingFigurePointer = *it;
                    t = tempT.getValue();
                }
            }
        }
    }
    if (foundIntersections) {
        double intensity = 0;
        for (auto it = lighters.begin(); it != lighters.end(); ++it) {

            Ray lightRay(ray.getPoint(t), it->getPlace());
            bool shines = true;
            for (auto jt = figures.begin(); jt != figures.end(); ++jt) {
                if ((*jt) == intersectingFigurePointer) {
                    continue;
                }
                Optional<double> currentT = (*jt)->getT(lightRay);
                if (currentT.hasValue()) {
                    if (currentT.getValue() < lightRay.getPointT(it->getPlace())) {
                        shines = false;
                        break;
                    }
                }
            }
            if (shines) {
                double k = fabs(scalarProduct(intersectingFigurePointer->normal(lightRay.start), lightRay.direction));
                intensity += it->intensity(ray.getPoint(t)) * k;
            }
        }
        return intersectingFigurePointer->getColor().normalized(intensity);
    } else {
        return ColorRGB();
    }
}

void Scene::process() {
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            pixels[x][y].setColor(runRay(pixels[x][y]));
        }
    }
}

void Scene::antialiase() {
    std::vector< std::vector<Pixel> > oldPixels;
    oldPixels.reserve(width);
    for (size_t i = 0; i < pixels.size(); ++i) {
        oldPixels.push_back(std::vector<Pixel>());
        oldPixels[i].reserve(height);
        for (size_t j = 0; j < pixels[i].size(); ++j) {
            oldPixels[i].push_back(pixels[i][j]);
        }
    }

    for (size_t i = 0; i < pixels.size(); ++i) {
        for (size_t j = 0; j < pixels[i].size(); ++j) {
            std::vector<ColorRGB> mixingColors;
            mixingColors.push_back(oldPixels[i][j].getColor());
            if ((i > 0) && (j > 0)) {
                mixingColors.push_back(oldPixels[i - 1][j - 1].getColor());
            }
            if (i > 0) {
                mixingColors.push_back(oldPixels[i - 1][j].getColor());
            }
            if ((i > 0) && (j < pixels.size() - 1)) {
                mixingColors.push_back(oldPixels[i - 1][j + 1].getColor());
            }
            if (j > 0) {
                mixingColors.push_back(oldPixels[i][j - 1].getColor());
            }
            if (j < pixels.size() - 1) {
                mixingColors.push_back(oldPixels[i][j + 1].getColor());
            }
            if ((j > 0) && (i < pixels.size() - 1)) {
                mixingColors.push_back(oldPixels[i + 1][j - 1].getColor());
            }
            if (i < pixels.size() - 1) {
                mixingColors.push_back(oldPixels[i + 1][j].getColor());
            }
            if ((i < pixels.size() - 1) && (j < pixels.size() - 1)) {
                mixingColors.push_back(oldPixels[i + 1][j + 1].getColor());
            }
            pixels[i][j].setColor(ColorRGB(mixingColors));
        }
    }
}

void Scene::save(const char* file) const {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);
    for (size_t i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            cairo_set_source_rgb(cr, pixels[i][j].getRed(), pixels[i][j].getGreen(), pixels[i][j].getBlue());
            cairo_rectangle(cr, pixels[i][j].getX(), pixels[i][j].getY(), 1, 1);
            cairo_fill(cr);
        }
    }

    cairo_surface_write_to_png(surface, file);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

#endif
