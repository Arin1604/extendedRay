#include "utils/sceneparser.h"
#include "raytracer/raytracescene.h"
#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>
#include "Sphere.h"

/*!
    @param:
    *float a,b,c: representing the coefficients of x^2, x and the constant term c of a quadratic equation

    @return:
    * float t value

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
float sphere::quadraticHelper(float a, float b, float c){
    float t = -1;
    float det = pow(b, 2) - 4 * a * c;


    if(det < 0){
        return t;
    }



    else{// Since both tangential and full on intersection count as intersection
        float sol1 = (-b + sqrt(det))/ (2*a);
        float sol2 = (-b - sqrt(det))/ (2*a);

        if(sol1 < 0 || sol2 < 0){
            t = fmax(sol1, sol2);
        }



        else if(sol2> 0 && sol1>0){
            t = fmin(sol1, sol2);
        }

        return t;
    }

}


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the insident light rau

    @return:
    * RayTracer::intersectInfo:- returns the intersect information for the shape and light ray

    @brief:
    * Function for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::intersectInfo sphere::getSphereIntersection(RayTracer::Ray ray){
    float t;
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;


    //glm::vec4 q = P + t * D;
    float a = pow(D[0], 2) + pow(D[1], 2) + pow(D[2], 2);
    float b = 2 * (P[0] * D[0] + P[1]*D[1] + P[2]*D[2]);
    float c = pow(P[0], 2) + pow(P[1], 2) + pow(P[2], 2) - pow(0.5, 2);// Radius or 1?


    t = quadraticHelper(a,b,c);
    glm::vec4 pos = P + t * D;
    glm::vec4 norm1(2*pos[0], 2*pos[1], 2*pos[2], 1.0f);
    float z = P[2] + t*D[2];
    float x = P[0] + t * D[0];


    float theta = atan2(z, x);
    float u;
    if(theta < 0){
        u = -theta/(2.f * M_PI);
    }
    else{
        u = 1 -theta/(2.f * M_PI);
    }

    float phi = asinf(pos[1]/0.5f);
    float v = (phi/M_PI) + (1.0f/2.0f);

    RayTracer::intersectInfo intersectInfo = {t, pos, norm1, u, v};

    return intersectInfo ;

}
