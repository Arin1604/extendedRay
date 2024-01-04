#pragma once

#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>


class sphere
{
public:
    static RayTracer::intersectInfo getSphereIntersection(RayTracer::Ray ray);

private:
    static float quadraticHelper(float a, float b, float c);


};
