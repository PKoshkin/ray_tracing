#ifndef COLOR_KOSHKIN_493 
#define COLOR_KOSHKIN_493

class Color {
private:
    double red;
    double green;
    double blue;
public:
    Color();
    Color(double inRed, double inGreen, double inBlue);
    Color(const Color& color);
    double getRed() const;
    double getGreen() const;
    double getBlue() const;
};

Color::Color() {
    // Цвет точки по-умолчанию - черный
    red = 0;
    green = 0;
    blue = 0;
}

Color::Color(double inRed, double inGreen, double inBlue) : red(inRed), green(inGreen), blue(inBlue) {}

Color::Color(const Color& color) : red(color.red), green(color.green), blue(color.blue) {}

double Color::getRed() const {
    return red;
}

double Color::getGreen()  const {
    return green;
}

double Color::getBlue() const {
    return blue;
}

#endif
