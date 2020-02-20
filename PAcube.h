#pragma once
#include "prefab.h"
#include "quaternion.h"
#include <iostream>

std::string to_string(Vector v) {
    return '(' + std::to_string(v.x) + ',' + std::to_string(v.y) + ',' + std::to_string(v.z) + ')';
}
std::string to_string(Quaternion q) {
    return '(' + std::to_string(q.a) + ',' + std::to_string(q.b) + ',' + std::to_string(q.c) + ',' + std::to_string(q.d) + ')';
}

const float framerate = 20.;
static const float PI = 4 * atan(1.);

Quaternion projZ(Quaternion q) {
    if (q.b == 0. && q.c == 0.)
        return q;
    float theta = acos(q.a);
    float v3 = q.d / sin(theta);
    float ang = tan(theta) * v3;
    ang = 2 * atan(ang);
    std::cout << ang << ' ';
    return rotation(ang, Vector(0, 0, 1));
}

struct Square {
    Vector position;
    Quaternion rotation;
    Object* mainObj, * auxObj;
    bool visible = true;
    int color = 0;
    float side;
    float currMainRot = 0.;
    float currAuxRot = 0.;
    Square(float side = 1.) {
        this->side = side;
        mainObj = new Object(0., 15, shapes::sq3, 0, false, false, false);
        auxObj = new Object(0., 15, shapes::sq, 0, false, true, true);
        mainObj->posEvent(0., 0., 0.);
        mainObj->scaEvent(0., side, side);
        mainObj->rotEvent(0., 0.);
        mainObj->colEvent(0., 0);
        auxObj->posEvent(0., 0., 0.);
        auxObj->scaEvent(0., 1., 1.);
        auxObj->rotEvent(0., 0.);
        auxObj->colEvent(0., 0);
        mainObj->parent = auxObj;
        mainObj->parentOffset(true, true, true);
    }
    void update(float t) {
        if (visible)
            mainObj->scaEvent(t, side, side, easeTypes::inst);
        else
            mainObj->scaEvent(t, 0., 0., easeTypes::inst);
        Quaternion rotZ, rotXY;
        rotZ = projZ(rotation);
        rotXY = rotZ.inv() * rotation;
        // Position
        auxObj->posEvent(t, position.x, position.y);
        // First rotation
        float XYvecAng = atan2(rotXY.c, rotXY.b);
        float angDiff = -XYvecAng - currMainRot;
        std::cout << (angDiff * 180 / PI) << ' ';
        if ((angDiff > PI / 2 && angDiff < 3 * PI / 2) || (angDiff<-PI / 2 && angDiff>-3 * PI / 2)) { // II or III quadrants
            rotZ = -rotZ;
            rotXY = -rotXY;
            XYvecAng = atan2(rotXY.c, rotXY.b);
            angDiff = -XYvecAng - currMainRot;
        }
        float XYang = 2 * acos(rotXY.a);
        while (angDiff > PI)
            angDiff -= 2 * PI;
        while (angDiff < -PI)
            angDiff += 2 * PI;
        std::cout << rotZ.d << ' ' << (angDiff * 180 / PI) << std::endl;
        mainObj->rotEvent(t, angDiff * 180 / PI);
        currMainRot = -XYvecAng;
        auxObj->scaEvent(t, 1., cos(XYang));
        // Second rotation
        float Zang = 2 * acos(rotZ.a * (rotZ.d > 0 ? 1 : -1));
        angDiff = XYvecAng + Zang - currAuxRot;
        while (angDiff > PI)
            angDiff -= 2 * PI;
        while (angDiff < -PI)
            angDiff += 2 * PI;
        auxObj->rotEvent(t, angDiff * 180 / PI);
        currAuxRot = XYvecAng + Zang;

    }
};

Vector cubeSidePositions[6] = {
    Vector(0.,0.,1.),
    Vector(0.,1.,0.),
    Vector(1.,0.,0.),
    Vector(0.,-1.,0.),
    Vector(-1.,0.,0.),
    Vector(0.,0.,-1.)
};

Quaternion cubeSideRotations[6] = {
    Quaternion(1.,0.,0.,0.),
    rotation(-PI / 2,Vector(1.,0.,0.)),
    rotation(PI / 2,Vector(0.,1.,0.)),
    rotation(PI / 2,Vector(1.,0.,0.)),
    rotation(-PI / 2,Vector(0.,1.,0.)),
    rotation(-PI,Vector(1.,0.,0.))
};

struct Cube {
    Vector position;
    Quaternion rotation;
    Square* sides[6];
    float edge = 16.;
    Cube() {
        for (int i = 0; i < 6; i++) {
            sides[i] = new Square(edge);
            Object* obj = new Object(0., 16, shapes::sq3, 0, false, false, false);
            obj->posEvent(0., 0., 0.);
            obj->scaEvent(0., 16., 16.);
            obj->rotEvent(0., 0.);
            obj->colEvent(0., 1);
            obj->parent = sides[i]->mainObj;
        }
    }
    void update(float t) {
        for (int i = 0; i < 6; i++) {
            sides[i]->position = position + .5 * edge * rotate(cubeSidePositions[i], rotation);
            sides[i]->rotation = rotation * cubeSideRotations[i];
            sides[i]->visible = sides[i]->position.z > position.z;
            std::cout << i << ' ' << sides[i]->position.z << ' ';
            sides[i]->update(t);
        }
    }
};
