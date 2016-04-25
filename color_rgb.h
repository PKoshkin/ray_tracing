#ifndef COLOR_RGB_KOSHKIN_493 
#define COLOR_RGB_KOSHKIN_493

#include <vector>

class ColorRGB {
private:
    double red;
    double green;
    double blue;

public:
    ColorRGB();
    ColorRGB(double inRed, double inGreen, double inBlue);
    ColorRGB(const ColorRGB& color);
    ColorRGB(const std::vector<ColorRGB>& colors); // Микс цветов

    double getRed() const;
    double getGreen() const;
    double getBlue() const;

    void normalize(double alpha);
    ColorRGB normalized(double alpha) const;
    void set(double inRed, double inGreen, double inBlue);
};

// Цвет точки по-умолчанию - черный
ColorRGB::ColorRGB() : red(0), green(0), blue(0) {}

ColorRGB::ColorRGB(double inRed, double inGreen, double inBlue) : red(inRed), green(inGreen), blue(inBlue) {}

ColorRGB::ColorRGB(const ColorRGB& color) : red(color.red), green(color.green), blue(color.blue) {}

ColorRGB::ColorRGB(const std::vector<ColorRGB>& colors) {
    red = green = blue = 0;
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        red += it->red;
        green += it->green;
        blue += it->blue;
    }
    red /= colors.size();
    green /= colors.size();
    blue /= colors.size();
}

double ColorRGB::getRed() const {
    return red;
}

double ColorRGB::getGreen()  const {
    return green;
}

double ColorRGB::getBlue() const {
    return blue;
}

void ColorRGB::normalize(double alpha) {
    red *= alpha;
    green *= alpha;
    blue *= alpha;
}

ColorRGB ColorRGB::normalized(double alpha) const {
    return ColorRGB(red * alpha, green * alpha, blue * alpha);
}

void ColorRGB::set(double inRed, double inGreen, double inBlue) {
    red = inRed;
    green = inGreen;
    blue = inBlue;
}

#endif
