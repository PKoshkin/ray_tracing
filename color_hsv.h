#ifndef COLOR_HSV_KOSHKIN_493 
#define COLOR_HSV_KOSHKIN_493

class ColorHSV {
private:
    double hue;
    double saturation;
    double value;

public:
    ColorHSV(double inHue, double inSaturation, double inValue);
    ColorHSV(const ColorHSV& color);

    double getHue() const;
    double getSaturation() const;
    double getValue() const;

    void setValue(double inValue);
};

ColorHSV::ColorHSV(double inHue, double inSaturation, double inValue) : hue(inHue), saturation(inSaturation), value(inValue) {}

ColorHSV::ColorHSV(const ColorHSV& color) : hue(color.hue), saturation(color.saturation), value(color.value) {}

double ColorHSV::getHue() const {
    return hue;
}

double ColorHSV::getSaturation() const {
    return saturation;
}

double ColorHSV::getValue() const {
    return value;
}

void ColorHSV::setValue(double inValue) {
    value = inValue;
}

#endif
