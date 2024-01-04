#pragma once

#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>

///
/// \brief The sphere class represents the sphere as a mathematical object
///        using implicit equations.
///

class sphere
{
public:
    static RayTracer::intersectInfo getSphereIntersection(RayTracer::Ray ray);

private:
    static float quadraticHelper(float a, float b, float c);


};
