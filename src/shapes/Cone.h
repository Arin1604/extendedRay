#pragma once

#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>


///
/// \brief The cone class represents the cone as a mathematical object
///        using 2 implicit equations: one for the lateral surface and
///        one for the base.
///
class cone
{
public:
    static RayTracer::intersectInfo getConeIntersection(RayTracer::Ray ray);

private:
    static float quadraticHelperCone(float a, float b, float c, glm::vec4 P, glm::vec4 D);
    static RayTracer::surfaceStruct getConeBodyIntersection(RayTracer::Ray ray);


};
