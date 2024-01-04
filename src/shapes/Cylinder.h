#pragma once

#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>

///
/// \brief The cylinder class represents the cylinder as a mathematical object
///        using implicit equations.
///

class cylinder
{
public:
    static RayTracer::intersectInfo getCylinderIntersection(RayTracer::Ray ray);
    static RayTracer::surfaceStruct getCircularBaseIntersection(RayTracer::Ray ray, float plane);

private:
    static float quadraticHelper(float a, float b, float c);
    static RayTracer::surfaceStruct getCylinderBodyIntersection(RayTracer::Ray ray);


};
