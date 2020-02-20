#include <iostream>
#include <string>
#include "PAcube.h"

float interTime = 2.;
Vector positions[7] = {
    Vector(0.,0.,0.),
    Vector(0.,0.,0.),
    Vector(0.,0.,0.),
    Vector(2.,2.,1.),
    Vector(-3.,-1.,-1.),
    Vector(1.,1.,2.),
    Vector(0.,0.,0.)
};
Vector rotationAxes[6] = {
    Vector(1.,2.,0.),
    Vector(0.,-2.,1.),
    Vector(5.,3.,-3.),
    Vector(1.,1.,1.),
    Vector(1.,-3.,2.),
    Vector(1.,2.,0.)
};
float rotationAngles[6] = {
    PI / 3,PI / 4,-PI,2 * PI / 3,PI,3 * PI / 4
};

int main()
{
    prefab::name = "Cube";
    prefab::type = prefabTypes::Misc1;

    Cube* cube = new Cube();
    //Square *cube = new Square();
    float currTime = 0.;
    Quaternion currRotation = Quaternion(1, 0, 0, 0);
    for (int i = 0; i < 6; i++) {
        for (float localTime = 0; localTime < interTime; localTime += 1. / framerate) {
            float param = localTime / interTime;
            cube->position = (1 - param) * positions[i] + param * positions[i + 1];
            cube->rotation = rotation(param * rotationAngles[i], rotationAxes[i]) * currRotation;
            cube->update(currTime + localTime);
        }
        currTime += interTime + 1.;
        currRotation = rotation(rotationAngles[i], rotationAxes[i]) * currRotation;
    }
    prefab::generate();
    return 0;
}