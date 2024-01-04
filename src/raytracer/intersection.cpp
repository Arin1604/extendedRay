#include "shapes/Sphere.h"
#include "utils/sceneparser.h"
#include "raytracescene.h"
#include "raytracer.h"
#include <iostream>
#include <cmath>

RayTracer::surfaceStruct RayTraceScene::getminPos(std::vector<RayTracer::surfaceStruct> &t_StrucArray){
    RayTracer::surfaceStruct t_min_pos_Struc = t_StrucArray[0];
    for(RayTracer::surfaceStruct t : t_StrucArray){
        if(t.t >= 0.0f && (t.t < t_min_pos_Struc.t || t_min_pos_Struc.t < 0.0f)){
            t_min_pos_Struc = t;
        }
    }

    return t_min_pos_Struc;
}

float quadraticHelper(float a, float b, float c){
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

float quadraticHelperCone(float a, float b, float c, glm::vec4 P, glm::vec4 D){
    float t = -1;
    float det = pow(b, 2) - 4 * a * c;

    //    std::cout<<  det << std::endl;
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



RayTracer::surfaceStruct getCylinderBodyIntersection(RayTracer::Ray ray){
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

    //    float phi = asinf(pos[1]/0.5f);
    float v = y + 0.5f;

    RayTracer::surfaceStruct struc = {t1, normal};
    struc.u = u;
    struc.v = v;

    return struc;






}

RayTracer::surfaceStruct getCircularBaseIntersection(RayTracer::Ray ray, float plane){
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


    //    if(plane<0){        // DOUBLE CHECK IF THIS CAN BE GENERALIZED FOR ALL PLANES IN Y
    //        normal = {0,-1,0,0};
    //    }

    //    else{
    //        normal = {0, 1, 0, 0};
    //    }

    RayTracer::surfaceStruct bodyStruc = {t, normal};


    bodyStruc.u = u;
    bodyStruc.v = v;

    return bodyStruc;
}

RayTracer::intersectInfo getCylinderIntersection(RayTracer::Ray ray){
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

RayTracer::surfaceStruct getConeBodyIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;


    float a =  pow(D[0], 2) + pow(D[2], 2) - ((1.0f/4.0f) * pow(D[1], 2));
    float b = 2 * (P[0] * D[0] + P[2]*D[2]) - (1.0f/2.0f) * P[1] * D[1] + (1.0f/4.0f) * D[1];
    float c = pow(P[0], 2) + pow(P[2], 2) - (1.0f/4.0f) * pow(P[1], 2) + (1.0f/4.0f) * P[1] - 1.0f/16.0f;



    float t = quadraticHelperCone(a,b,c, P, D);


//    if(P[1] + t * D[1] > 0.5 || P[1] + t * D[1] < -0.5 ){
//        t = -1;

//    }

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

    //    float phi = asinf(pos[1]/0.5f);
    float v = y + 0.5f;

    RayTracer::surfaceStruct bodyStruc = {t, normal};
    bodyStruc.u = u;
    bodyStruc.v = v;

    return bodyStruc;


}

RayTracer::intersectInfo getConeIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    RayTracer::surfaceStruct t = getConeBodyIntersection(ray);
    RayTracer::surfaceStruct t1 = getCircularBaseIntersection(ray, -0.5f);

    std::vector<RayTracer::surfaceStruct> t_StrucArray{t, t1};
    RayTracer::surfaceStruct tFinal = RayTraceScene::getminPos(t_StrucArray);


    glm::vec4 pos = P + tFinal.t * D;
    glm::vec4 norm1 = tFinal.normal;
    //std::cout << tFinal.normal[0] << "," << tFinal.normal[1] << "," << tFinal.normal[2] << std::endl;
    glm::vec3 norm3 = glm::normalize((glm::vec3) norm1);
    glm::vec4 norm = glm::vec4(norm3, 0.f);
    //std::cout << norm[0] << "," <<norm[1] << "," << norm[2] << std::endl;
    RayTracer::intersectInfo intersectInfo = {tFinal.t, pos, norm, tFinal.u, tFinal.v};//still not 0;

    //std::cout << intersectInfo.normal[0] << "," << intersectInfo.normal[1] << "," << intersectInfo.normal[2] << std::endl;
    return intersectInfo;

}


RayTracer::surfaceStruct getCubeXIntersect(RayTracer::Ray ray, float plane){
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

    //glm::vec4 intersect = P + t* D;

    //std::cout << y << "   " << z << std::endl;


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
    //std::cout << u << "   " << v << std::endl;

    return struc;



}

RayTracer::surfaceStruct getCubeYIntersect(RayTracer::Ray ray, float plane){
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

RayTracer::surfaceStruct getCubeZIntersect(RayTracer::Ray ray, float plane){
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


RayTracer::intersectInfo getCubeIntersection(RayTracer::Ray ray){
    glm::vec4 P = ray.pos;
    glm::vec4 D = ray.dir;

    RayTracer::surfaceStruct tx1 = getCubeXIntersect(ray, 0.5);
    RayTracer::surfaceStruct tx2 = getCubeXIntersect(ray, -0.5);

    RayTracer::surfaceStruct ty1 = getCubeYIntersect(ray, 0.5);
    RayTracer::surfaceStruct ty2 = getCubeYIntersect(ray, -0.5);

    RayTracer::surfaceStruct tz1 = getCubeZIntersect(ray, 0.5);
    RayTracer::surfaceStruct tz2 = getCubeZIntersect(ray, -0.5);

    //    std::cout << tx1.normal.x << tx1.normal.y << tx1.normal.z << std::endl;
    //    std::cout << tx2.normal.x << tx2.normal.y << tx2.normal.z << std::endl;


    std::vector<RayTracer::surfaceStruct> t_StrucArray{tx1, tx2, ty1, ty2, tz1, tz2};
    RayTracer::surfaceStruct tFinal = RayTraceScene::getminPos(t_StrucArray);

    glm::vec4 pos = P + tFinal.t * D;
    glm::vec4 norm1 = tFinal.normal;
    glm::vec4 norm = glm::normalize(norm1);
    RayTracer::intersectInfo intersectInfo = {tFinal.t, pos, norm, tFinal.u, tFinal.v};
    return intersectInfo;

}




RayTracer::intersectInfo RayTraceScene::getIntersection(RenderShapeData shape, RayTracer::Ray ray){
    RayTracer::intersectInfo intersectInfo;
    if(shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE){
        intersectInfo = sphere::getSphereIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER){
        intersectInfo = getCylinderIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CONE){
        intersectInfo = getConeIntersection(ray);
    }
    else{
        intersectInfo = getCubeIntersection(ray);

    }


    return intersectInfo;


}
