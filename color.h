#ifndef COLOR_KOSHKIN_493 
#define COLOR_KOSHKIN_493

#include <algorithm> // Для std::max

#include "color_rgb.h"
#include "color_hsv.h"

ColorRGB HSVToRGB(const ColorHSV& color) {
    int hueI = int(color.getHue() / 60) % 6;
    double vMin = (1 - color.getSaturation()) * color.getValue();
    double vInc = vMin + (color.getValue() - vMin) * (int(color.getHue()) % 60) / 60;
    double vDec = color.getValue() - (color.getValue() - vMin) * (int(color.getHue()) % 60) / 60;

    double red, green, blue;
    switch (hueI) {
        case 0:
            red = color.getValue();
            green = vInc;
            blue = vMin;
        break;
        case 1:
            red = vDec;
            green = color.getValue();
            blue = vMin;
        break;
        case 2:
            red = vMin;
            green = color.getValue();
            blue = vInc;
        break;
        case 3:
            red = vMin;
            green = vDec;
            blue = color.getValue();
        break;
        case 4:
            red = vInc;
            green = vMin;
            blue = color.getValue();
        break;
        case 5:
            red = color.getValue();
            green = vMin;
            blue = vDec;
        break;
    }
    return ColorRGB(red, green, blue);
}

ColorHSV RGBToHSV(const ColorRGB& color) {
    double max = std::max(color.getRed(), std::max(color.getGreen(), color.getBlue()));
    double min = std::min(color.getRed(), std::min(color.getGreen(), color.getBlue()));

    double hue, saturation, value;

    value = max;
    saturation = (max == 0) ? 0 : 1 - min / max;

    if (max == min) {
        hue = 0;
    } else if (max == color.getRed()) {
        hue = 60 * (color.getGreen() - color.getBlue()) / (max - min);
        if (color.getGreen() < color.getBlue()) {
            hue += 360;
        }
    } if (max == color.getGreen()) {
        hue = 60 * (color.getBlue() - color.getRed()) / (max - min) + 120;
    } else {
        hue = 60 * (color.getRed() - color.getGreen()) / (max - min) + 240;
    }
    return ColorHSV(hue, saturation, value);
}

#endif
