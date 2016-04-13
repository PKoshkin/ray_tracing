#ifndef SCREEN_KOSHKIN_493 
#define SCREEN_KOSHKIN_493

#include <vector>
#include <cairo.h>

#include "pixel.h"

class Screen {
private:
    int height;
    int width;
    std::vector< std::vector< Pixel > > pixels;

public:
    Screen(int inWidth, int inHeight);
    void setColor(int x, int y, const Color& color);
    void draw() const;
};

Screen::Screen(int inWidth, int inHeight) : width(inWidth), height(inHeight) {
    pixels.reserve(width);
    for (int i = 0; i < width; ++i) {
        pixels.push_back(std::vector<Pixel>());
        pixels[i].reserve(height);
        for (int j = 0; j < height; ++j) {
            pixels[i].push_back(Pixel(i, j));
        }
    }
}

void Screen::setColor(int x, int y, const Color& color) {
    pixels[x][y].setColor(color);
}

void Screen::draw() const {
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
