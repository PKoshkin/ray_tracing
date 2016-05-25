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
public:
    // Разрешение экрана
    double height;
    double width;

    Point camera; // Точка в которой стоит камера
    Vector vectorToCenter; // Вектор от камеры до центра экрана

    // Базис в плоскости экрана
    Vector axisX;
    Vector axisY;

    std::vector< std::vector<Pixel> > pixels;
    std::vector< std::shared_ptr<Figure> > figures;
    std::vector<Lighter> lighters;

public:
    Scene() {}
    Scene(double inWidth, double inHeight, Vector inVectorToCenter, Point inCamera, Vector inAxisX, Vector inAxisY);
    void addFigure(std::shared_ptr<Figure> figure);
    void addLighter(const Lighter& inLighter);
    Ray rayToPixel(const Pixel& pixel) const;
    ColorRGB runRay(const Pixel& pixel) const;
    void process(int screenWidth, int screenHeight);
    void save(const char* file) const;
    void antialiase();
};

Scene::Scene(double inWidth, double inHeight, Vector inVectorToCenter, Point inCamera, Vector inAxisX, Vector inAxisY) :
    height(inHeight), width(inWidth), vectorToCenter(inVectorToCenter), camera(inCamera), axisX(inAxisX.normalized()), axisY(inAxisY.normalized()) {}

void Scene::addFigure(std::shared_ptr<Figure> figure) {
    figures.emplace_back(std::move(figure));
}

void Scene::addLighter(const Lighter& inLighter) {
    lighters.push_back(inLighter);
}

Ray Scene::rayToPixel(const Pixel& pixel) const {
    double xStep = width / pixels.size();
    double yStep = height / pixels[0].size();
    Vector directionVector = vectorToCenter + axisX * (pixel.getX() * xStep - width / 2) + axisY * (pixel.getY() * yStep - height / 2);
    return Ray((Vector(camera) + directionVector).getEnd(), directionVector);
}

ColorRGB Scene::runRay(const Pixel& pixel) const {
    Ray ray = rayToPixel(pixel);

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

void Scene::process(int screenWidth, int screenHeight) {
    // Создаем экран нужного размера
    pixels.reserve(screenWidth);
    for (size_t i = 0; i < screenWidth; ++i) {
        pixels.push_back(std::vector<Pixel>());
        pixels[i].reserve(screenHeight);
        for (size_t j = 0; j < screenHeight; ++j) {
            pixels[i].push_back(Pixel(i, j));
        }
    }

    // Считаем цвета пикселей
    for (size_t x = 0; x < screenWidth; ++x) {
        for (size_t y = 0; y < screenHeight; ++y) {
            pixels[x][y].setColor(runRay(pixels[x][y]));
        }
    }
}

void Scene::antialiase() {
    std::vector< std::vector<Pixel> > oldPixels;
    oldPixels.reserve(pixels.size());
    for (size_t i = 0; i < pixels.size(); ++i) {
        oldPixels.push_back(std::vector<Pixel>());
        oldPixels[i].reserve(pixels[0].size());
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
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, pixels.size(), pixels[0].size());
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
