#ifndef COLOR_LAB_KOSHKIN_493 
#define COLOR_LAB_KOSHKIN_493

class ColorLAB {
private:
    double l;
    double a;
    double b;

public:
    ColorLAB(double inL, double inA, double inB);
    ColorLAB(const ColorLAB& color);

    double getL() const;
    double getA() const;
    double getB() const;
};

ColorLAB::ColorLAB(double inL, double inA, double inB) : l(inL), a(inA), b(inB) {}

ColorLAB::ColorLAB(const ColorLAB& color) : l(color.l), a(color.a), b(color.b) {}

double ColorLAB::getL() const {
    return l;
}

double ColorLAB::getA()  const {
    return a;
}

double ColorLAB::getB() const {
    return b;
}

#endif
