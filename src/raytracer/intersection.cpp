#include "shapes/Sphere.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
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


RayTracer::intersectInfo RayTraceScene::getIntersection(RenderShapeData shape, RayTracer::Ray ray){
    RayTracer::intersectInfo intersectInfo;
    if(shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE){
        intersectInfo = sphere::getSphereIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER){
        intersectInfo = cylinder::getCylinderIntersection(ray);
    }
    else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CONE){
        intersectInfo = cone::getConeIntersection(ray);
    }
    else{
        intersectInfo = cube::getCubeIntersection(ray);

    }


    return intersectInfo;


}
