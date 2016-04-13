#ifndef VECTOR_KOSHKIN_493 
#define VECTOR_KOSHKIN_493

#include <fstream>

#include "point.h"

class Vector {
private:
    // Конец вектора. Можно было бы не писать отдельный класс для вектора. Но идеалогически так приятнее.
    Point end;

public:
    Vector(double inX, double inY, double inZ);
    Vector(const Point& point);
    Vector(const Vector& vector);

    Vector operator+(const Vector& vector) const;
    Vector operator-(const Vector& vector) const;

    friend std::ostream& operator<<(std::ostream& out, const Vector& vector);

    friend Vector vectorProduct(const Vector& vector1, const Vector& vector2);
};

Vector::Vector(double inX, double inY, double inZ) : end(inX, inY, inZ) {}

Vector::Vector(const Point& point) : end(point) {}

Vector::Vector(const Vector& vector) : end(vector.end.getX(), vector.end.getY(), vector.end.getZ()) {}

Vector Vector::operator+(const Vector& vector) const {
    return Vector(end.getX() + vector.end.getX(), end.getY() + vector.end.getY(), end.getZ() + vector.end.getZ());
}

Vector Vector::operator-(const Vector& vector) const {
    return Vector(end.getX() - vector.end.getX(), end.getY() - vector.end.getY(), end.getZ() - vector.end.getZ());
}

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
    out << "(" << vector.end.getX() << ", " << vector.end.getY() << ", " << vector.end.getZ() << ")";
    return out;
}


Vector vectorProduct(const Vector& vector1, const Vector& vector2) {
    return Vector(
        vector1.end.getY() * vector2.end.getZ() - vector2.end.getY() * vector1.end.getZ(),
        -(vector1.end.getX() * vector2.end.getZ() - vector2.end.getX() * vector1.end.getZ()),
        vector1.end.getX() * vector2.end.getY() - vector2.end.getX() * vector1.end.getY()
    );
}

#endif
