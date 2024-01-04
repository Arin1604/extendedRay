#include "raytracer/raytracescene.h"
#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>
#include "Cube.h"


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *float plane:- the (+/-) planes in the YZ plane that bound the cube's x vals

    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the cube's x faces

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::surfaceStruct cube::getCubeXIntersect(RayTracer::Ray ray, float plane){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;
    float u;
    float v;

    float t = (plane - P[0])/D[0];




    float y = P[1] + t * D[1];
    float z = P[2] + t * D[2];

    if(y < -0.5 || y > 0.5 || z < -0.5 || z > 0.5){
        t = -1;
    }


    glm::vec4 normal(0.f,0.f,0.f,0.f);//using if elses check for +j^ or -j^
    if(plane>0){
        normal = glm::vec4{1.f, 0.f, 0.f, 0.f};
        u = -z + 0.5f;
        v = y + 0.5f;
    }
    else {
        normal = glm::vec4{-1.f, 0.f, 0.f, 0.f};
        u = z + 0.5f;
        v = y + 0.5f;
    }

    RayTracer::surfaceStruct struc = {t, normal};
    struc.u = u;
    struc.v = v;
    return struc;



}

/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *float plane:- the (+/-) planes in the XZ plane that bound the cube's y vals

    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the cube's y faces

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/

RayTracer::surfaceStruct cube::getCubeYIntersect(RayTracer::Ray ray, float plane){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    float t = (plane - P[1])/D[1];

    float u;
    float v;

    //Check for x and z bund
    float x = P[0] + t * D[0];
    float z = P[2] + t * D[2];
    if(x < -0.5 || x > 0.5 || z < -0.5 || z > 0.5){
        t = -1;
    }

    glm::vec4 normal(0.f,0.f,0.f,0.f);//using if elses check for +j^ or -j^
    if(plane>0){
        normal = glm::vec4{0.f, 1.f, 0.f, 0.f};
        u = x + 0.5f;
        v = 1 -(z + 0.5f);
    }

    else {
        normal = glm::vec4{0.f, -1.f, 0.f, 0.f};
        u = x + 0.5f;
        v = z + 0.5f;
    }

    RayTracer::surfaceStruct struc = {t, normal};
    struc.u = u;
    struc.v = v;
    return struc;


}


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *float plane:- the (+/-) planes in the XY plane that bound the cube's z vals

    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the cube's z faces

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::surfaceStruct cube::getCubeZIntersect(RayTracer::Ray ray, float plane){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    float t = (plane - P[2])/D[2];
    float u;
    float v;

    float x = P[0] + t * D[0];
    float y = P[1] + t * D[1];

    if(x < -0.5 || x > 0.5 || y < -0.5 || y > 0.5){
        t = -1;
    }

    glm::vec4 normal(0.f,0.f,0.f,0.f);//using if elses check for +j^ or -j^
    if(plane>0){
        normal = glm::vec4{0.f, 0.f, 1.f, 0.f};
        u = x + 0.5f;
        v = y + 0.5f;
    }

    else {
        normal = glm::vec4{0.f, 0.f, -1.f, 0.f};
        u = -x + 0.5f;/*(1 -(x + 0.5f))*/;
        v = (y + 0.5f);
    }

    RayTracer::surfaceStruct struc = {t, normal};
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
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1
    * Uses the face specific helpers to define a cube

*/
RayTracer::intersectInfo cube::getCubeIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    RayTracer::surfaceStruct tx1 = getCubeXIntersect(ray, 0.5);
    RayTracer::surfaceStruct tx2 = getCubeXIntersect(ray, -0.5);

    RayTracer::surfaceStruct ty1 = getCubeYIntersect(ray, 0.5);
    RayTracer::surfaceStruct ty2 = getCubeYIntersect(ray, -0.5);

    RayTracer::surfaceStruct tz1 = getCubeZIntersect(ray, 0.5);
    RayTracer::surfaceStruct tz2 = getCubeZIntersect(ray, -0.5);


    std::vector<RayTracer::surfaceStruct> t_StrucArray{tx1, tx2, ty1, ty2, tz1, tz2};
    RayTracer::surfaceStruct tFinal = RayTraceScene::getminPos(t_StrucArray);

    glm::vec4 pos = P + tFinal.t * D;
    glm::vec4 norm1 = tFinal.normal;
    glm::vec4 norm = glm::normalize(norm1);
    RayTracer::intersectInfo intersectInfo = {tFinal.t, pos, norm, tFinal.u, tFinal.v};
    return intersectInfo;

}


