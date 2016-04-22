#ifndef COLOR_XYZ_KOSHKIN_493 
#define COLOR_XYZ_KOSHKIN_493

class ColorXYZ {
private:
    double x;
    double y;
    double z;

public:
    ColorXYZ(double inX, double inY, double inZ);
    ColorXYZ(const ColorXYZ& color);

    double getX() const;
    double getY() const;
    double getZ() const;
};

ColorXYZ::ColorXYZ(double inX, double inY, double inZ) : x(inX), y(inY), z(inZ) {}

ColorXYZ::ColorXYZ(const ColorXYZ& color) : x(color.x), y(color.y), z(color.z) {}

double ColorXYZ::getX() const {
    return x;
}

double ColorXYZ::getY()  const {
    return y;
}

double ColorXYZ::getZ() const {
    return z;
}

#endif
