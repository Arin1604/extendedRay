#pragma once

#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>


///
/// \brief The cube class represents the cube as a mathematical object
///        using 6 implicit equations for each of its faces.
///
class cube
{
public:
    static RayTracer::intersectInfo getCubeIntersection(RayTracer::Ray ray);

private:
    static RayTracer::surfaceStruct getCubeZIntersect(RayTracer::Ray ray, float plane);
    static RayTracer::surfaceStruct getCubeXIntersect(RayTracer::Ray ray, float plane);
    static RayTracer::surfaceStruct getCubeYIntersect(RayTracer::Ray ray, float plane);

};
