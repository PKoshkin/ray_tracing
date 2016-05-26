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
#include "intersection.h"
#include "optional.h"
#include "constants.h"
#include "lighter.h"
#include "ray.h"
#include "3d_tree.h"

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
    Tree3D tree;
    std::vector<Lighter> lighters;

public:
    Scene() {}
    Scene(double inWidth, double inHeight, Vector inVectorToCenter, Point inCamera, Vector inAxisX, Vector inAxisY);
    void setTree(const Tree3D& inTree);
    void addLighter(const Lighter& inLighter);
    Ray rayToPixel(const Pixel& pixel) const;
    ColorRGB runRay(const Pixel& pixel) const;
    void process(int screenWidth, int screenHeight);
    void save(const char* file) const;
    void antialiase(size_t times = 1);
    void whiteBalance();
};

Scene::Scene(double inWidth, double inHeight, Vector inVectorToCenter, Point inCamera, Vector inAxisX, Vector inAxisY) :
    height(inHeight), width(inWidth), vectorToCenter(inVectorToCenter), camera(inCamera), axisX(inAxisX.normalized()), axisY(inAxisY.normalized()) {}


void Scene::setTree(const Tree3D& inTree) {
    tree = inTree;
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

    Optional<Intersection> intersection = tree.getIntersection(ray);
    if (intersection.hasValue()) {
        double intensity = 0;
        for (auto it = lighters.begin(); it != lighters.end(); ++it) {

            Ray lightRay(ray.getPoint(intersection.getValue().t), it->getPlace());
            Optional<Intersection> intersectionToLighter = tree.getIntersection(lightRay);
            if (intersectionToLighter.hasValue()) {
                if (intersectionToLighter.getValue().t < lightRay.getPointT(it->getPlace())) {
                    double k = fabs(scalarProduct(intersection.getValue().figure->normal(lightRay.start), lightRay.direction));
                    intensity += it->intensity(ray.getPoint(intersection.getValue().t)) * k;
                } else {
                    std::cout << "I wona this string" << std::endl;
                }
            } else {

                //std::cout << "don't have value!" << std::endl;

                double k = fabs(scalarProduct(intersection.getValue().figure->normal(lightRay.start), lightRay.direction));
                intensity += it->intensity(ray.getPoint(intersection.getValue().t)) * k;
            }
        }
        return intersection.getValue().figure->getColor().normalized(intensity);
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

void Scene::antialiase(size_t times) {
    for (size_t time = 0; time < times; ++time) {
        // Сохраняем старые пиксели
        std::vector< std::vector<Pixel> > oldPixels;
        oldPixels.reserve(pixels.size());
        for (size_t i = 0; i < pixels.size(); ++i) {
            oldPixels.push_back(std::vector<Pixel>());
            oldPixels[i].reserve(pixels[0].size());
            for (size_t j = 0; j < pixels[i].size(); ++j) {
                oldPixels[i].push_back(pixels[i][j]);
            }
        }

        // Новые пиксели - среднее арифметическое всех близлежащих.
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
}

void Scene::whiteBalance() {
    double maxColor = 0;
    for (size_t i = 0; i < pixels.size(); ++i) {
        for (size_t j = 0; j < pixels[i].size(); ++j) {
            if (pixels[i][j].getRed() > maxColor) {
                maxColor = pixels[i][j].getRed();
            }
            if (pixels[i][j].getGreen() > maxColor) {
                maxColor = pixels[i][j].getGreen();
            }
            if (pixels[i][j].getBlue() > maxColor) {
                maxColor = pixels[i][j].getBlue();
            }
        }
    }
    for (size_t i = 0; i < pixels.size(); ++i) {
        for (size_t j = 0; j < pixels[i].size(); ++j) {
            pixels[i][j].setColor(pixels[i][j].getColor().normalized(1 / maxColor));
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
