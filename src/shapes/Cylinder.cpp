#include "raytracer/raytracescene.h"
#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>
#include "Cylinder.h"



/*!
    @param:
    *float a,b,c: representing the coefficients of x^2, x and the constant term c of a quadratic equation

    @return:
    * float t value

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
float cylinder::quadraticHelper(float a, float b, float c){
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
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *float plane:- the plane in which the circle lies

    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the circular base

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::surfaceStruct cylinder::getCircularBaseIntersection(RayTracer::Ray ray, float plane){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;
    float t = (plane - P[1])/D[1];
    float x = P[0] + t * D[0];
    float z = P[2] + t * D[2];
    if(!(pow(x, 2) + pow(z,2) <= pow(0.5,2))){
        t = -1;
    }
    float u;
    float v;

    glm::vec4 normal(0.f,0.f,0.f,0.f);//Norm not getting accessed for cone?
    if(plane < 0){
        normal = {0.f, -1.f, 0.f, 0.f};
        u = x + 0.5f;
        v = z + 0.5f;
    }
    else{
        normal = {0.f, 1.f, 0.f, 0.f};
        u = x + 0.5f;
        v = -z + 0.5f;
    }


    RayTracer::surfaceStruct bodyStruc = {t, normal};


    bodyStruc.u = u;
    bodyStruc.v = v;

    return bodyStruc;
}

/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *
    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the cylindrical body

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::surfaceStruct cylinder::getCylinderBodyIntersection(RayTracer::Ray ray){
    float t1;

    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    float a = pow(D[0], 2) + pow(D[2], 2);
    float b = 2 * (P[0] * D[0] + P[2]*D[2]);
    float c = pow(P[0], 2) + pow(P[2], 2) - pow(0.5, 2);

    t1 = quadraticHelper(a,b,c);
    if(P[1] + t1 * D[1] > 0.5 || P[1] + t1 * D[1] < -0.5 ){
        t1 = -1;

    }

    glm::vec4 pos = P + t1 * D;
    float x = P[0] + t1 * D[0];
    float z = P[2] + t1 * D[2];
    float y = P[1] + t1 * D[1];
    glm::vec4 normal(2 * pos[0], 0.f, 2 * pos[2], 0.f);//Modify calculations

    float theta = atan2(z, x);
    float u;
    if(theta < 0){
        u = -theta/(2.f * M_PI);
    }
    else{
        u = 1 -theta/(2.f * M_PI);
    }

    float v = y + 0.5f;

    RayTracer::surfaceStruct struc = {t1, normal};
    struc.u = u;
    struc.v = v;

    return struc;
}


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray

    @return:
    * RayTracer::intersectInfo:- returns the intersect information for the shape and light ray

    @brief:
    * Function for returning the closest real intersection point
    * If no real intersection, returns -1
    * Uses the cylinder body and base helpers to define a cylinder

*/
RayTracer::intersectInfo cylinder::getCylinderIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    RayTracer::surfaceStruct t = getCylinderBodyIntersection(ray);
    RayTracer::surfaceStruct t1 = getCircularBaseIntersection(ray, -0.5f);
    RayTracer::surfaceStruct t2 = getCircularBaseIntersection(ray, +0.5f);

    std::vector<RayTracer::surfaceStruct> t_StrucArray{t, t1, t2};
    RayTracer::surfaceStruct tFinal = RayTraceScene::getminPos(t_StrucArray);

    glm::vec4 pos = P + tFinal.t * D;
    glm::vec4 norm1 = tFinal.normal;
    glm::vec4 norm = glm::normalize(norm1);
    RayTracer::intersectInfo intersectInfo = {tFinal.t, pos, norm, tFinal.u, tFinal.v};

    return intersectInfo;

}

