#include "raytracer/raytracescene.h"
#include "raytracer/raytracer.h"
#include <iostream>
#include <cmath>
#include "Cone.h"
#include "Cylinder.h"


/*!
    @param:
    *float a,b,c: representing the coefficients of x^2, x and the constant term c of a quadratic equation
    *glm::vec4 P, D: the position and direction vectors of the incident light ray

    @return:
    * float t value

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1
    * Checks the special case for the cone's apex (because the implicit equations of a cone don't sufficently describe the apex)

*/
float cone::quadraticHelperCone(float a, float b, float c, glm::vec4 P, glm::vec4 D){
    float t = -1;
    float det = pow(b, 2) - 4 * a * c;

    if(det < 0){
        return t;
    }





    else{// Since both tangential and full on intersection count as intersection
        float sol1 = (-b + sqrt(det))/ (2*a);
        float sol2 = (-b - sqrt(det))/ (2*a);
        float y1 = P[1] + sol1 * D[1];
        float y2 = P[1] + sol2 * D[1];

        if(sol1 < 0 || sol2 < 0){
            return -1;
        }


        if((y1 < -0.5 || y1 > 0.5) && (y2 < -0.5 || y2 > 0.5)){
            return -1;

        }

        else if(y1 < -0.5 || y1 > 0.5){
            return sol2;

        }

        else if(y2 < -0.5 || y2 > 0.5){
            return sol1;

        }

        else{
            return fmin(sol1,sol2);
        }

        return t;
    }
}


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray
    *
    @return:
    *RayTracer::surfaceStruct:- the surface intersection information for the cone's lateral body

    @brief:
    * Helper for returning the closest real intersection point
    * If no real intersection, returns -1

*/
RayTracer::surfaceStruct cone::getConeBodyIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;


    float a =  pow(D[0], 2) + pow(D[2], 2) - ((1.0f/4.0f) * pow(D[1], 2));
    float b = 2 * (P[0] * D[0] + P[2]*D[2]) - (1.0f/2.0f) * P[1] * D[1] + (1.0f/4.0f) * D[1];
    float c = pow(P[0], 2) + pow(P[2], 2) - (1.0f/4.0f) * pow(P[1], 2) + (1.0f/4.0f) * P[1] - 1.0f/16.0f;



    float t = quadraticHelperCone(a,b,c, P, D);



    glm::vec4 pos = P + t * D;

    glm::vec4 normal(2 * pos[0], -(pos[1] / 2.0f)+0.25f, 2 * pos[2], 0.f);//Norm not getting accessed
    float x = P[0] + t * D[0];
    float z = P[2] + t * D[2];
    float y = P[1] + t * D[1];
    float theta = atan2(z, x);
    float u;
    if(theta < 0){
        u = -theta/(2.f * M_PI);
    }
    else{
        u = 1 -theta/(2.f * M_PI);
    }


    float v = y + 0.5f;

    RayTracer::surfaceStruct bodyStruc = {t, normal};
    bodyStruc.u = u;
    bodyStruc.v = v;

    return bodyStruc;

}


/*!
    @param:
    *RayTracer::Ray ray:- represents the Ray struct for the incident light ray

    @return:
    * RayTracer::intersectInfo:- returns the intersect information for the shape and light ray

    @brief:
    * Function for returning the closest real intersection point
    * If no real intersection, returns -1
    * Uses the cone body and base helpers to define a cone implicitly

*/
RayTracer::intersectInfo cone::getConeIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    RayTracer::surfaceStruct t = getConeBodyIntersection(ray);
    RayTracer::surfaceStruct t1 = cylinder::getCircularBaseIntersection(ray, -0.5f);

    std::vector<RayTracer::surfaceStruct> t_StrucArray{t, t1};
    RayTracer::surfaceStruct tFinal = RayTraceScene::getminPos(t_StrucArray);


    glm::vec4 pos = P + tFinal.t * D;
    glm::vec4 norm1 = tFinal.normal;
    glm::vec3 norm3 = glm::normalize((glm::vec3) norm1);
    glm::vec4 norm = glm::vec4(norm3, 0.f);
    RayTracer::intersectInfo intersectInfo = {tFinal.t, pos, norm, tFinal.u, tFinal.v};//still not 0;

    return intersectInfo;

}


