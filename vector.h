#ifndef VECTOR_KOSHKIN_493 
#define VECTOR_KOSHKIN_493

#include <fstream>
#include <math.h>

#include "point.h"

class Vector {
private:
    // Конец вектора. Можно было бы не писать отдельный класс для вектора. Но идеалогически так приятнее.
    Point end;

public:
    Vector();
    Vector(double inX, double inY, double inZ);
    Vector(const Point& point);
    Vector(const Vector& vector);

    void operator=(const Vector& vector);
    Vector operator+(const Vector& vector) const;
    Vector operator-(const Vector& vector) const;
    friend Vector operator*(const Vector& vector, double alpha);
    friend Vector operator*(double alpha, const Vector& vector);

    Point getEnd() const;
    double length() const;
    double squareLength() const;
    void normalize();
    Vector normalized() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector& vector);

    friend Vector vectorProduct(const Vector& vector1, const Vector& vector2);
    friend double scalarProduct(const Vector& vector1, const Vector& vector2);
};

Vector::Vector() {}

Vector::Vector(double inX, double inY, double inZ) : end(inX, inY, inZ) {}

Vector::Vector(const Point& point) : end(point) {}

Vector::Vector(const Vector& vector) : end(vector.end.getX(), vector.end.getY(), vector.end.getZ()) {}

void Vector::operator=(const Vector& vector) {
    end = vector.end;
}

Vector Vector::operator+(const Vector& vector) const {
    return Vector(end.getX() + vector.end.getX(), end.getY() + vector.end.getY(), end.getZ() + vector.end.getZ());
}

Vector Vector::operator-(const Vector& vector) const {
    return Vector(end.getX() - vector.end.getX(), end.getY() - vector.end.getY(), end.getZ() - vector.end.getZ());
}

Vector operator*(const Vector& vector, double alpha) {
    return Vector(vector.end.getX() * alpha, vector.end.getY() * alpha, vector.end.getZ() * alpha);
}

Vector operator*(double alpha, const Vector& vector) {
    return vector * alpha;
}

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
    out << "(" << vector.end.getX() << ", " << vector.end.getY() << ", " << vector.end.getZ() << ")";
    return out;
}

Point Vector::getEnd() const {
    return end;
}

double Vector::length() const {
    return sqrt((end.getX()) * (end.getX()) + (end.getY()) * (end.getY()) + (end.getZ()) * (end.getZ())); 
}

double Vector::squareLength() const {
    return (end.getX()) * (end.getX()) + (end.getY()) * (end.getY()) + (end.getZ()) * (end.getZ()); 
}

void Vector::normalize() {
    end = Point(end.getX() / length(), end.getY() / length(), end.getZ() / length());
}

Vector Vector::normalized() const {
    return Vector(end.getX() / length(), end.getY() / length(), end.getZ() / length());
}

Vector vectorProduct(const Vector& vector1, const Vector& vector2) {
    return Vector(
        vector1.end.getY() * vector2.end.getZ() - vector2.end.getY() * vector1.end.getZ(),
        -(vector1.end.getX() * vector2.end.getZ() - vector2.end.getX() * vector1.end.getZ()),
        vector1.end.getX() * vector2.end.getY() - vector2.end.getX() * vector1.end.getY()
    );
}

double scalarProduct(const Vector& vector1, const Vector& vector2) {
    return vector1.getEnd().getX() * vector2.getEnd().getX() + vector1.getEnd().getY() * vector2.getEnd().getY() + vector1.getEnd().getZ() * vector2.getEnd().getZ();
}

#endif
