#pragma once
#include <math.h>

struct Vector {
    float x, y, z;
    Vector(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector() : Vector(0., 0., 0.) {}
    Vector operator - () {
        return Vector(-x, -y, -z);
    }
    float norm2() {
        return x * x + y * y + z * z;
    }
    float norm() {
        return sqrt(norm2());
    }
};

Vector operator + (Vector v1, Vector v2) {
    return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vector operator - (Vector v1, Vector v2) {
    return v1 + -v2;
}
Vector operator * (float f, Vector v) {
    return Vector(f * v.x, f * v.y, f * v.z);
}
Vector operator * (Vector v, float f) {
    return f * v;
}
Vector operator / (Vector v, float f) {
    return 1 / f * v;
}
float dot(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vector cross(Vector v1, Vector v2) {
    return Vector(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

struct Quaternion {
    float a, b, c, d;
    Quaternion(float a, float b, float c, float d) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }
    Quaternion(float re, Vector im) {
        this->a = re;
        this->b = im.x;
        this->c = im.y;
        this->d = im.z;
    }
    Quaternion() : Quaternion(1., 0., 0., 0.) {}
    float re() {
        return a;
    }
    Vector im() {
        return Vector(b, c, d);
    }
    Quaternion operator - () {
        return Quaternion(-a, -b, -c, -d);
    }
    float norm2() {
        return a * a + b * b + c * c + d * d;
    }
    float norm() {
        return sqrt(norm2());
    }
    Quaternion inv() {
        float f = norm2();
        return Quaternion(a / f, -b / f, -c / f, -d / f);
    }
};

Quaternion operator + (Quaternion q1, Quaternion q2) {
    return Quaternion(q1.a + q2.a, q1.b + q2.b, q1.c + q2.c, q1.d + q2.d);
}
Quaternion operator - (Quaternion q1, Quaternion q2) {
    return q1 + -q2;
}
Quaternion operator * (Quaternion q1, Quaternion q2) {
    float re1 = q1.re();
    float re2 = q2.re();
    Vector im1 = q1.im();
    Vector im2 = q2.im();
    return Quaternion(
        re1 * re2 - dot(im1, im2),
        re1 * im2 + re2 * im1 + cross(im1, im2)
    );
}
Quaternion operator / (Quaternion q1, Quaternion q2) {
    return q1 * q2.inv();
}
Quaternion rotation(float a, Vector v) {
    return Quaternion(cos(a / 2), sin(a / 2) * v / v.norm());
}
Vector rotate(Vector v, Quaternion q) {
    return (q * Quaternion(0, v) / q).im();
}